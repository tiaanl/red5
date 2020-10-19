#include "scene.h"

#include <spdlog/spdlog.h>

#include <cassert>

namespace {

void renderImageToBuffer(const Image& image, SDL_Color* palette, SDL_Color* pixels) {
  auto imageWidth = image.width();
  auto imageHeight = image.height();

  for (auto& line : image.lines()) {
    MemSize y = line.top - image.top();

    if (y >= imageHeight) {
      continue;
    }

    MemSize x = line.left - image.left();
    MemSize pos = y * imageWidth + x;
    for (auto index : line.indices) {
      pixels[pos++] = palette[index];
      if (++x >= imageWidth) {
        break;
      }
    }
  }
}

SDL_Texture* createTextureFromImage(SDL_Renderer* renderer, SDL_Color* palette,
                                    const Image& image) {
  U16 imageWidth = image.width();
  U16 imageHeight = image.height();

  std::vector<SDL_Color> buffer;
  buffer.resize(imageWidth * imageHeight);
  std::memset(buffer.data(), 0, buffer.size() * sizeof(SDL_Color));

  renderImageToBuffer(image, palette, buffer.data());

  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
      buffer.data(), imageWidth, imageHeight, 32, static_cast<I32>(imageWidth * sizeof(SDL_Color)),
      SDL_PIXELFORMAT_RGBA32);

  return SDL_CreateTextureFromSurface(renderer, surface);
}

}  // namespace

Scene::Scene(SDL_Renderer* renderer) : m_renderer{renderer}, m_palette{} {}

void Scene::addResources(const ResourceFile& resourceFile) {
  auto entries = resourceFile.loadEntries();

  for (auto& entry : entries) {
    spdlog::info("entry: {} ({})", entry.name(), resourceTypeToString(entry.type()));
    m_entries.emplace_back(std::move(entry));
  }
}

bool Scene::loadPalette(std::string_view name) {
  auto palette = loadResource<Palette>(m_entries, ResourceType::Palette, name);
  if (!palette) {
    return false;
  }

  applyPalette(*palette);

  return true;
}

bool Scene::loadFilm(std::string_view name) {
  auto film = loadResource<Film>(m_entries, ResourceType::Film, name);
  if (!film) {
    return false;
  }

  m_film.swap(film);

  processFilm();

  return true;
}

void Scene::update(U32 millis) {
  static const U32 millisPerFrame = 1000 / 8;

  m_totalMillis += millis;

  if (m_totalMillis > millisPerFrame) {
    m_totalMillis %= millisPerFrame;

    // Advance the scene frame.
    m_currentFrame += 1;
    m_currentFrame %= m_frameCount;

    for (auto& prop : m_props) {
      prop.nextFrame(m_currentFrame);
    }
  }
}

void Scene::render() {
  assert(m_film);

  std::sort(std::begin(m_props), std::end(m_props),
            [](auto& left, auto& right) { return right.layer() < left.layer(); });

  for (auto& prop : m_props) {
    prop.render(m_renderer);
  }
}

void Scene::applyPalette(const Palette& palette) {
  U8 i = palette.firstIndex();
  for (auto& c : palette.colors()) {
    m_palette[i++] = SDL_Color{c.red, c.green, c.blue, 255};
  }
}

void Scene::processFilm() {
  m_frameCount = m_film->frameCount();

  for (auto& block : m_film->blocks()) {
    switch (block.type) {
      case BlockType::View: {
        processViewBlock(block);
        break;
      }

      case BlockType::Pltt: {
        processPaletteBlock(block);
        break;
      }

      case BlockType::Delt: {
        processImageBlock(block);
        break;
      }

      case BlockType::Anim: {
        processAnimationBlock(block);
        break;
      }

      default: {
        // assert(false);
        break;
      }
    }
  }

  // Switch to frame 0 for all props.
  for (auto& prop : m_props) {
    prop.nextFrame(0);
  }
}

void Scene::processViewBlock(const Film::Block& block) {}

void Scene::processPaletteBlock(const Film::Block& block) {
  auto palette = loadResource<Palette>(m_entries, ResourceType::Palette, block.name);
  if (!palette) {
    spdlog::warn("Palette not found: {}", block.name);
    return;
  }

  // TODO: We need to process the `opCode`s, but for now we just apply the palette.

  applyPalette(*palette);
}

void Scene::processImageBlock(const Film::Block& block) {
  auto image = loadResource<Image>(m_entries, ResourceType::Image, block.name);
  if (!image) {
    spdlog::warn("Image not found: {}", block.name);
    return;
  }

  std::vector<RenderItem> renderItems;
  SDL_Texture* texture = createTextureFromImage(m_renderer, m_palette, *image);
  SDL_Rect rect{image->left(), image->top(), image->width(), image->height()};
  renderItems.emplace_back(texture, rect);

  m_props.emplace_back(block.chunks, std::move(renderItems));
}

void Scene::processAnimationBlock(const Film::Block& block) {
  auto animation = loadResource<Animation>(m_entries, ResourceType::Animation, block.name);
  if (!animation) {
    spdlog::warn("Animation not found: {}", block.name);
    return;
  }

  std::vector<RenderItem> renderItems;
  for (auto& image : animation->frames()) {
    SDL_Texture* texture = createTextureFromImage(m_renderer, m_palette, image);
    SDL_Rect rect{image.left(), image.top(), image.width(), image.height()};
    renderItems.emplace_back(texture, rect);
  }

  m_props.emplace_back(block.chunks, std::move(renderItems));
}
