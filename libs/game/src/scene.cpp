#include "game/scene.h"

#include <imgui.h>

namespace game {

namespace {

U16 imageStride(U16 width) {
  return (width + 3u) & ~3u;
}

void renderImageToBuffer(U8* buffer, const Image& image) {
  auto imageHeight = image.height();

  //  spdlog::info("image :: left: {}, top: {}, right: {}, bottom: {}", image.left(), image.top(),
  //               image.right(), image.bottom());

#if 0
  for (auto& line : image.lines()) {
    U16 y = line.left - image.left();
    for (int p = 0; p < y; ++p) {
      printf("   ");
    }
    for (U8 ch : line.indices) {
      printf("%*d ", 2, ch);
    }
    printf("\n");
  }
#endif  // 0

  for (auto& line : image.lines()) {
    //    spdlog::info("line :: left: {}, top: {}, size: {}", line.left, line.top,
    //    line.indices.size());

    U16 y = line.top - image.top();
    U16 x = line.left - image.left();

    if (y >= imageHeight) {
      continue;
    }

    U16 stride = imageStride(image.width());

    auto start = y * stride + x;
    // spdlog::info("start: {}", start);
    std::copy(line.indices.data(), line.indices.data() + line.indices.size(), &buffer[start]);
  }
}

renderer::TextureId createIndexTexture(renderer::Renderer* renderer, const Image& image) {
  if (image.lines().empty()) {
    U8 empty = 0;
    return renderer->textures().create(&empty, renderer::TextureFormat::Red, {1, 1});
  }

  U16 imageWidth = image.width();
  U16 imageHeight = image.height();
  U16 stride = imageStride(image.width());

  std::vector<U8> indices;
  indices.resize(stride * imageHeight);
  std::memset(indices.data(), 0, indices.size());

  renderImageToBuffer(indices.data(), image);

  return renderer->textures().create(indices.data(), renderer::TextureFormat::Red,
                                     {imageWidth, imageHeight});
}

}  // namespace

Scene::Scene(SceneDelegate* sceneDelegate, Resources* resources, SceneRenderer* sceneRenderer)
  : m_delegate{sceneDelegate},
    m_resources{resources},
    m_sceneRenderer{sceneRenderer},
    m_palette{} {}

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
  m_font->load(m_sceneRenderer->renderer(), *font);

  return true;
}

PropId Scene::insertImage(std::string_view name, std::vector<Film::Chunk> chunks) {
  auto* resource = m_resources->findResource(ResourceType::Image, name);
  if (!resource) {
    return PropId::invalidValue();
  }

  auto image = loadResource<Image>(*resource);
  if (!image) {
    return PropId::invalidValue();
  }

  return insertImageProp(name, *image, std::move(chunks));
}

PropId Scene::insertAnimation(std::string_view name, std::vector<Film::Chunk> chunks) {
  auto* resource = m_resources->findResource(ResourceType::Animation, name);
  if (!resource) {
    return PropId::invalidValue();
  }

  auto animation = loadResource<Animation>(*resource);
  if (!animation) {
    return PropId::invalidValue();
  }

  return insertAnimationProp(name, *animation, std::move(chunks));
}

void Scene::update(U32 millis) {
  static const U32 millisPerFrame = 1000 / 8;

  m_totalMillis += millis;

  if (m_totalMillis > millisPerFrame) {
    m_totalMillis %= millisPerFrame;

    advanceToNextFrame();
  }
}

void Scene::renderGameScreen() {
  std::sort(std::begin(m_renderOrder), std::end(m_renderOrder),
            [this](const PropId& left, const PropId& right) {
              Prop* leftData = m_props.getData(left);
              Prop* rightData = m_props.getData(right);

              return rightData->layer() < leftData->layer();
            });

  for (auto& propId : m_renderOrder) {
    auto prop = m_props.getData(propId);
    if (prop) {
      prop->render(m_sceneRenderer);
    }
  }
}

void Scene::renderDebugInfo() {
  ImGui::Begin("Playback");
  ImGui::Text("Frame %d of %d", m_currentFrame, m_frameCount);
  if (ImGui::Button("Next Frame")) {
    advanceToNextFrame();
  }
  ImGui::End();

  ImGui::Begin("Resources");
  if (ImGui::Button("Insert")) {
    if (m_selectedResourceType == ResourceType::Image) {
      insertImage(m_selectedEntry, {});
    } else if (m_selectedResourceType == ResourceType::Animation) {
      insertAnimation(m_selectedEntry, {});
    }
  }
  for (auto& entry : *m_resources) {
    auto name = fmt::format("{} :: {}", resourceTypeToString(entry.type()), entry.name().data());
    bool selected = entry.type() == m_selectedResourceType && entry.name() == m_selectedEntry;
    if (ImGui::Selectable(name.data(), selected)) {
      m_selectedResourceType = entry.type();
      m_selectedEntry = entry.name();
    }
  }
  ImGui::End();

  ImGui::Begin("Scene");

  for (auto& propId : m_renderOrder) {
    auto* prop = m_props.getData(propId);
    if (prop) {
      I32 layer = prop->m_layer;
      I32 currentFrame = prop->m_currentSpriteIndex;
      I32 direction = prop->m_animation.direction;

      auto name = fmt::format("{}##{}", prop->name(), propId.id);
      if (ImGui::TreeNode(name.data())) {
        ImGui::Checkbox("Visible", &prop->m_visible);
        ImGui::SliderInt("Layer", &layer, 0, 100);
        ImGui::SliderInt("Current frame", &currentFrame, 0,
                         static_cast<I32>(prop->m_sprites.size()) - 1);
        ImGui::InputInt2("Offset X", (int*)&prop->m_offset);
        ImGui::LabelText("Animation", "dir: %d, rate: %d", prop->m_animation.direction,
                         prop->m_animation.frameRate);

        if (ImGui::TreeNode("chunks")) {
          for (auto& chunk : prop->m_chunks) {
            switch (chunk.opCode) {
              case OpCode::End:
                ImGui::Text("End");
                break;

              case OpCode::Time:
                ImGui::Text("Time");
                break;

              case OpCode::Move:
                ImGui::Text("Move");
                break;

              case OpCode::Speed:
                ImGui::Text("Speed");
                break;

              case OpCode::Layer:
                ImGui::Text("Layer");
                break;

              case OpCode::Frame:
                ImGui::Text("Frame");
                break;

              case OpCode::Animation:
                ImGui::Text("Animation");
                break;

              case OpCode::Event:
                ImGui::Text("Event");
                break;

              case OpCode::Region:
                ImGui::Text("Region");
                break;

              case OpCode::Window:
                ImGui::Text("Window");
                break;

              case OpCode::Shift:
                ImGui::Text("Shift");
                break;

              case OpCode::Display:
                ImGui::Text("Display");
                break;

              case OpCode::Orientation:
                ImGui::Text("Orientation");
                break;

              case OpCode::Use:
                ImGui::Text("Use");
                break;

              case OpCode::Unknown11:
                ImGui::Text("Unknown11");
                break;

              case OpCode::Transition:
                ImGui::Text("Transition");
                break;

              case OpCode::Unknown12:
                ImGui::Text("Unknown12");
                break;

              case OpCode::Loop:
                ImGui::Text("Loop");
                break;

              case OpCode::Unknown17:
                ImGui::Text("Unknown17");
                break;

              case OpCode::Preload:
                ImGui::Text("Preload");
                break;

              case OpCode::Sound:
                ImGui::Text("Sound");
                break;

              case OpCode::Stereo:
                ImGui::Text("Stereo");
                break;
            }
            for (I16 var : chunk.variables) {
              ImGui::SameLine();
              ImGui::Text("%d", var);
            }
          }
          ImGui::TreePop();
        }

        ImGui::TreePop();
      }

      prop->m_layer = layer;
      prop->m_currentSpriteIndex = currentFrame;
      prop->m_animation.direction = direction;
    }
  }

  ImGui::End();
}

void Scene::applyPalette(const Palette& palette) {
  U8 i = palette.firstIndex();
  for (auto& c : palette.colors()) {
    m_palette[i++] = SDL_Color{c.red, c.green, c.blue, 255};
  }

  m_sceneRenderer->setPalette(reinterpret_cast<const renderer::RGB*>(palette.colors().data()),
                              palette.firstIndex(), palette.lastIndex());
}

PropId Scene::insertImageProp(std::string_view name, const Image& image,
                              std::vector<Film::Chunk> chunks) {
  std::vector<renderer::Sprite> sprites;

  auto texture = createIndexTexture(m_sceneRenderer->renderer(), image);
  if (!texture) {
    return PropId::invalidValue();
  }

  renderer::Rect rect{image.left(), image.top(), image.width(), image.height()};
  sprites.emplace_back(texture, rect);

  auto propId =
      m_props.create(ResourceType::Image, name, m_delegate, std::move(chunks), std::move(sprites));

  m_renderOrder.emplace_back(propId);

  return propId;
}

PropId Scene::insertAnimationProp(std::string_view name, const Animation& animation,
                                  std::vector<Film::Chunk> chunks) {
  std::vector<renderer::Sprite> sprites;

  for (auto& image : animation.frames()) {
    auto texture = createIndexTexture(m_sceneRenderer->renderer(), image);
    if (!texture) {
      return PropId::invalidValue();
    }

    renderer::Rect rect{image.left(), image.top(), image.width(), image.height()};
    sprites.emplace_back(texture, rect);
  }

  auto propId = m_props.create(ResourceType::Animation, name, m_delegate, std::move(chunks),
                               std::move(sprites));

  m_renderOrder.emplace_back(propId);

  return propId;
}

void Scene::processFilm() {
  m_frameCount = m_film->frameCount();

  for (auto& block : m_film->blocks()) {
    spdlog::info("Processing block: {}", block.name);
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

      case BlockType::End:
        break;

      default: {
        spdlog::info("Unknown block: ({}) {}", blockTypeToString(block.type), block.name);
        // assert(false);
        break;
      }
    }
  }

  // Set the current frame for each prop to 0.
  for (auto& propId : m_renderOrder) {
    auto* prop = m_props.getData(propId);
    if (prop) {
      prop->sceneTick(0);
    }
  }
}

void Scene::processViewBlock(const Film::Block& block) {
  for (auto& chunk : block.chunks) {
    switch (chunk.opCode) {
      case OpCode::Time:
        spdlog::info("  - {} :: {}", opCodeToString(chunk.opCode), chunk.variables[0]);
        break;

      case OpCode::Transition:
        // 1 == Swap
        // 2 == Clear
        // 3 == Dirty
        // 12 == FadeRight
        // 13 == FadeLeft
        // 14 == FadeUp
        // 16 == FadeDown
        // 21 is FadeUsDown
        // 22 is FadeToBlack
        // 23 is Stop

        spdlog::info("  - {} :: {}, {}", opCodeToString(chunk.opCode), chunk.variables[0],
                     chunk.variables[1]);
        break;

      default:
        spdlog::info("  - {}", opCodeToString(chunk.opCode));
        break;
    }
  }
}

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

  for (auto& chunk : block.chunks) {
    spdlog::info("chunk: {}", opCodeToString(chunk.opCode));
  }

  applyPalette(*palette);
}

void Scene::processImageBlock(const Film::Block& block) {
  //  if (block.name != "cursors") {
  //    return;
  //  }
  insertImage(block.name, block.chunks);
}

void Scene::processAnimationBlock(const Film::Block& block) {
  //  if (block.name != "cursors") {
  //    return;
  //  }
  insertAnimation(block.name, block.chunks);
}

void Scene::advanceToNextFrame() {
  if (m_currentFrame >= m_frameCount) {
    if (m_delegate) {
      m_delegate->onSceneLastFramePlayed();
    }

    return;
  }

  ++m_currentFrame;

  for (auto& propId : m_renderOrder) {
    auto prop = m_props.getData(propId);
    if (prop) {
      prop->sceneTick(m_currentFrame);
    }
  }
}

}  // namespace game
