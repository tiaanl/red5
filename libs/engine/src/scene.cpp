#include "engine/scene.h"

#include <cassert>

namespace engine {

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

renderer::TextureId createTextureFromImage(renderer::Renderer* renderer, SDL_Color* palette,
                                           const Image& image) {
  U16 imageWidth = image.width();
  U16 imageHeight = image.height();

  if (imageWidth <= 0 || imageWidth >= 512 || imageHeight <= 0 || imageHeight >= 512) {
    return renderer::TextureId::invalidValue();
  }

  std::vector<SDL_Color> buffer;
  buffer.resize(imageWidth * imageHeight);
  std::memset(buffer.data(), 0, buffer.size() * sizeof(SDL_Color));

  renderImageToBuffer(image, palette, buffer.data());

  return renderer->createTexture(buffer.data(), imageWidth, imageHeight);
}

}  // namespace

Scene::Scene(SceneDelegate* sceneDelegate, Resources* resources, renderer::Renderer* renderer)
  : m_delegate{sceneDelegate}, m_resources{resources}, m_renderer{renderer}, m_palette{} {}

bool Scene::loadPalette(std::string_view name) {
  auto* resource = m_resources->findResource(ResourceType::Palette, name);
  if (!resource) {
    return false;
  }

  auto palette = loadResource<Palette>(*resource);
  if (!palette) {
    return false;
  }

  applyPalette(*palette);

  return true;
}

bool Scene::loadFont(std::string_view name) {
  auto* resource = m_resources->findResource(ResourceType::Font, name);
  if (!resource) {
    return false;
  }
  auto font = loadResource<lfd::Font>(*resource);
  if (!font) {
    return false;
  }

  m_font = std::make_unique<Font>();
  m_font->load(m_renderer, *font);

  return true;
}

bool Scene::loadFilm(std::string_view name) {
  auto* resource = m_resources->findResource(ResourceType::Film, name);
  if (!resource) {
    return false;
  }
  auto film = loadResource<Film>(*resource);
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
    if (m_currentFrame >= m_frameCount) {
      if (m_delegate) {
        m_delegate->onSceneLastFramePlayed();
      }

      m_currentFrame = 0;
    }

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

  if (m_font) {
    m_font->renderText(m_renderer, {10, 10}, "TILO");
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
  auto* resource = m_resources->findResource(ResourceType::Palette, block.name);
  if (!resource) {
    return;
  }

  auto palette = loadResource<Palette>(*resource);
  if (!palette) {
    spdlog::warn("Palette not found: {}", block.name);
    return;
  }

  // TODO: We need to process the `opCode`s, but for now we just apply the palette.

  applyPalette(*palette);
}

void Scene::processImageBlock(const Film::Block& block) {
  auto* resource = m_resources->findResource(ResourceType::Image, block.name);
  if (!resource) {
    return;
  }

  auto image = loadResource<Image>(*resource);
  if (!image) {
    spdlog::warn("Image not found: {}", block.name);
    return;
  }

  std::vector<RenderItem> renderItems;
  auto texture = createTextureFromImage(m_renderer, m_palette, *image);
  if (!texture) {
    return;
  }

  renderer::Rect rect{image->left(), image->top(), image->width(), image->height()};
  renderItems.emplace_back(texture, rect);

  m_props.emplace_back(m_delegate, block.chunks, std::move(renderItems));
}

void Scene::processAnimationBlock(const Film::Block& block) {
  auto* resource = m_resources->findResource(ResourceType::Animation, block.name);
  if (!resource) {
    return;
  }

  auto animation = loadResource<Animation>(*resource);
  if (!animation) {
    spdlog::warn("Animation not found: {}", block.name);
    return;
  }

  std::vector<RenderItem> renderItems;
  for (auto& image : animation->frames()) {
    auto texture = createTextureFromImage(m_renderer, m_palette, image);
    renderer::Rect rect{image.left(), image.top(), image.width(), image.height()};
    renderItems.emplace_back(texture, rect);
  }

  m_props.emplace_back(m_delegate, block.chunks, std::move(renderItems));
}

}  // namespace engine
