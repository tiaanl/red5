#include "scene.h"

#include <lfd/image.h>
#include <lfd/palette.h>
#include <nucleus/Streams/ArrayInputStream.h>

#include <cassert>

namespace {

const ResourceEntry* findResource(const std::vector<ResourceEntry>& entries,
                                  ResourceType resourceType, std::string_view name) {
  auto it = std::find_if(std::begin(entries), std::end(entries), [&](const ResourceEntry& entry) {
    return entry.type() == resourceType && entry.name() == nu::StringView{name.data(), name.size()};
  });

  if (it == std::end(entries)) {
    return nullptr;
  }

  return &*it;
}

template <typename T>
std::unique_ptr<T> loadResource(const std::vector<ResourceEntry>& entries,
                                ResourceType resourceType, std::string_view name) {
  auto* resource = findResource(entries, resourceType, name);
  if (!resource) {
    return {};
  }

  nu::ArrayInputStream stream{nu::ArrayView{resource->data().data(), resource->data().size()}};

  auto result = std::make_unique<T>();
  result->read(&stream);

  return result;
}

std::unique_ptr<Image> loadImage(const nu::DynamicArray<ResourceEntry>& entries,
                                 nu::StringView name) {
  auto result = std::make_unique<Image>();

  auto imageResource =
      std::find_if(std::begin(entries), std::end(entries), [&name](const ResourceEntry& entry) {
        return entry.type() == ResourceType::Image && entry.name() == name;
      });
  if (imageResource != std::end(entries)) {
    nu::ArrayInputStream stream{
        nu::ArrayView{imageResource->data().data(), imageResource->data().size()}};
    result->read(&stream);
  }

  return result;
}

void drawImage(const Image& image, SDL_Color* palette, SDL_Color* pixels, U16 screenWidth) {
  // LOG(Info) << "Drawing image at (" << image.left() << ", " << image.top() << ")";
  for (auto& line : image.lines()) {
    MemSize pos = line.top * screenWidth + line.left;
    for (auto index : line.indices) {
      pixels[pos++] = palette[index];
    }
  }
}

}  // namespace

ImageSceneProp::ImageSceneProp(std::unique_ptr<Image> image) : m_image{std::move(image)} {}

void ImageSceneProp::render(const RenderState& renderState) {
  drawImage(*m_image, renderState.palette, renderState.pixels, renderState.screenWidth);
}

Scene::Scene() : m_palette{} {}

void Scene::addResources(const ResourceFile& resourceFile) {
  auto entries = resourceFile.loadEntries();

  for (auto& entry : entries) {
    m_resources.emplace_back(std::move(entry));
  }
}

bool Scene::loadFilm(std::string_view name) {
  auto film = loadResource<Film>(m_resources, ResourceType::Film, name);

  m_film.swap(film);

  processFilm();

  return true;
}

void Scene::render(SDL_Color* pixels) {
  assert(m_film);

  RenderState renderState{m_palette, m_width, m_height, pixels};

  for (auto& prop : m_props) {
    prop->render(renderState);
  }
}

ResourceEntry* Scene::findResource(ResourceType resourceType, std::string_view name) {
  auto it =
      std::find_if(std::begin(m_resources), std::end(m_resources), [&](const ResourceEntry& entry) {
        return entry.type() == resourceType &&
               entry.name() == nu::StringView{name.data(), name.size()};
      });

  if (it == std::end(m_resources)) {
    return nullptr;
  }

  return &*it;
}

void Scene::processFilm() {
  for (auto& block : m_film->blocks()) {
    // LOG(Info) << "Block: " << blockTypeToString(block.type);
    switch (block.type) {
      case BlockType::View:
        processViewBlock(block);
        break;

      case BlockType::Pltt:
        processPaletteBlock(block);
        break;

      case BlockType::Delt:
        processImageBlock(block);

      default:
        // assert(false);
        break;
    }
  }
}

void Scene::processViewBlock(const Film::Block& block) {
#if 0
  for (auto& chunk : block.chunks) {
    // LOG(Info) << opCodeToString(chunk.opCode);
    switch (chunk.opCode) {
      case OpCode::Time:
        // for (auto& var : chunk.variables) {
        //   LOG(Info) << var;
        // }
        break;

      case OpCode::Transition:
        // for (auto& var : chunk.variables) {
        //   LOG(Info) << var;
        // }
        break;

      case OpCode::End:
        break;

      default:
        assert(false);
        break;
    }
  }
#endif  // 0
}

void Scene::processPaletteBlock(const Film::Block& block) {
  auto palette = loadResource<Palette>(m_resources, ResourceType::Palette, block.name);
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
  auto image = loadResource<Image>(m_resources, ResourceType::Image, block.name);
  if (!image) {
    LOG(Warning) << "Image not found: " << block.name;
    return;
  }
  m_props.emplace_back(std::make_unique<ImageSceneProp>(std::move(image)));
}
