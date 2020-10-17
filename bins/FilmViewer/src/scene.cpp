#include "scene.h"

#include <lfd/palette.h>

#include <cassert>

Scene::Scene() : m_palette{} {}

void Scene::addResources(const ResourceFile& resourceFile) {
  auto entries = resourceFile.loadEntries();

  for (auto& entry : entries) {
    // LOG(Info) << entry;
    m_entries.emplace_back(std::move(entry));
  }
}

bool Scene::loadFilm(std::string_view name) {
  auto film = loadResource<Film>(m_entries, ResourceType::Film, name);

  m_film.swap(film);

  processFilm();

  return true;
}

void Scene::update(U32 millis) {
  m_totalMillis += millis;

  if (m_totalMillis > 1000) {
    // LOG(Info) << "next frame";
    m_currentFrame += 1;
    m_totalMillis %= 1000;

    for (auto& prop : m_props) {
      prop->nextFrame(m_currentFrame);
    }
  }

}

void Scene::render(SDL_Color* pixels) {
  assert(m_film);

  RenderState renderState{m_palette, m_width, m_height, pixels};

  std::sort(std::begin(m_props), std::end(m_props),
            [](auto& left, auto& right) { return left->layer() < right->layer(); });

  for (auto& prop : m_props) {
    prop->render(renderState);
  }
}

void Scene::processFilm() {
  for (auto& block : m_film->blocks()) {
    // LOG(Info) << "Block: " << blockTypeToString(block.type);
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
}

void Scene::processViewBlock(const Film::Block& block) {}

void Scene::processPaletteBlock(const Film::Block& block) {
  auto palette = loadResource<Palette>(m_entries, ResourceType::Palette, block.name);
  if (!palette) {
    LOG(Warning) << "Palette not found: " << block.name;
    return;
  }

  // TODO: We need to process the `opCode`s, but for now we just apply the palette.

  U8 i = palette->firstIndex();
  for (auto& c : palette->colors()) {
    m_palette[i++] = SDL_Color{c.red, c.green, c.blue, 255};
  }
}

void Scene::processImageBlock(const Film::Block& block) {
  auto image = loadResource<Image>(m_entries, ResourceType::Image, block.name);
  if (!image) {
    LOG(Warning) << "Image not found: " << block.name;
    return;
  }

  m_props.emplace_back(std::make_unique<ImageProp>(block.chunks, std::move(image)));
}

void Scene::processAnimationBlock(const Film::Block& block) {
  auto animation = loadResource<Animation>(m_entries, ResourceType::Animation, block.name);
  if (!animation) {
    LOG(Warning) << "Animation not found: " << block.name;
    return;
  }

  m_props.emplace_back(std::make_unique<AnimationProp>(block.chunks, std::move(animation)));
}
