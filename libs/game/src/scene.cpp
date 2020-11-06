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

Prop* Scene::prop(PropId propId) {
  return m_props.getData(propId);
}

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
  auto film = loadResource<lfd::Film>(*resource);
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

PropId Scene::insertImage(std::string_view name, std::vector<lfd::KeyFrame> keyFrames) {
  auto* resource = m_resources->findResource(ResourceType::Image, name);
  if (!resource) {
    return PropId::invalidValue();
  }

  auto image = loadResource<Image>(*resource);
  if (!image) {
    return PropId::invalidValue();
  }

  return insertImageProp(name, *image, std::move(keyFrames));
}

PropId Scene::insertAnimation(std::string_view name, std::vector<lfd::KeyFrame> keyFrames) {
  auto* resource = m_resources->findResource(ResourceType::Animation, name);
  if (!resource) {
    return PropId::invalidValue();
  }

  auto animation = loadResource<Animation>(*resource);
  if (!animation) {
    return PropId::invalidValue();
  }

  return insertAnimationProp(name, *animation, std::move(keyFrames));
}

PropId Scene::getPropUnderMouse(I32 x, I32 y) {
  for (auto it = m_renderOrder.crbegin(), end = m_renderOrder.crend(); it != end; ++it) {
    auto propData = m_props.getData(*it);
    if (propData) {
      auto& currentFrame = propData->currentFrame();
      auto& sprite = propData->m_sprites[currentFrame.spriteIndex];
      if (currentFrame.visible && sprite.rect().contains({x, y})) {
        return *it;
      }
    }
  }

  return PropId::invalidValue();
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

              return rightData->currentFrame().layer < leftData->currentFrame().layer;
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
  ImGui::SliderInt("Current Frame", &m_currentFrame, 0, m_film->frameCount());
  if (ImGui::Button("Next Frame")) {
    advanceToNextFrame();
    ++m_currentFrame;
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
      const auto& frame = prop->currentFrame();
      bool visible = frame.visible;
      I32 layer = frame.layer;
      I32 currentFrame = frame.spriteIndex;
      I32 offset[2] = {frame.offset.left, frame.offset.top};

      auto name = fmt::format("{}##{}", prop->name(), propId.id);
      if (ImGui::TreeNode(name.data())) {
        ImGui::Checkbox("Visible", &visible);
        ImGui::SliderInt("Layer", &layer, 0, 100);
        ImGui::SliderInt("Current frame", &currentFrame, 0,
                         static_cast<I32>(prop->m_sprites.size()) - 1);
        ImGui::InputInt2("Offset X", offset);

        if (ImGui::TreeNode("keyFrames")) {
          for (auto& keyFrame : prop->m_keyFrames) {
            switch (keyFrame.opCode) {
              case lfd::OpCode::End:
                ImGui::Text("End");
                break;

              case lfd::OpCode::Time:
                ImGui::Text("Time");
                break;

              case lfd::OpCode::Move:
                ImGui::Text("Move");
                break;

              case lfd::OpCode::Speed:
                ImGui::Text("Speed");
                break;

              case lfd::OpCode::Layer:
                ImGui::Text("Layer");
                break;

              case lfd::OpCode::Frame:
                ImGui::Text("Frame");
                break;

              case lfd::OpCode::Animation:
                ImGui::Text("Animation");
                break;

              case lfd::OpCode::Event:
                ImGui::Text("Event");
                break;

              case lfd::OpCode::Region:
                ImGui::Text("Region");
                break;

              case lfd::OpCode::Window:
                ImGui::Text("Window");
                break;

              case lfd::OpCode::Shift:
                ImGui::Text("Shift");
                break;

              case lfd::OpCode::Display:
                ImGui::Text("Display");
                break;

              case lfd::OpCode::Orientation:
                ImGui::Text("Orientation");
                break;

              case lfd::OpCode::Use:
                ImGui::Text("Use");
                break;

              case lfd::OpCode::Unknown11:
                ImGui::Text("Unknown11");
                break;

              case lfd::OpCode::Transition:
                ImGui::Text("Transition");
                break;

              case lfd::OpCode::Unknown12:
                ImGui::Text("Unknown12");
                break;

              case lfd::OpCode::Loop:
                ImGui::Text("Loop");
                break;

              case lfd::OpCode::Unknown17:
                ImGui::Text("Unknown17");
                break;

              case lfd::OpCode::Preload:
                ImGui::Text("Preload");
                break;

              case lfd::OpCode::Sound:
                ImGui::Text("Sound");
                break;

              case lfd::OpCode::Stereo:
                ImGui::Text("Stereo");
                break;
            }
            for (I16 var : keyFrame.variables) {
              ImGui::SameLine();
              ImGui::Text("%d", var);
            }
          }
          ImGui::TreePop();
        }

        ImGui::TreePop();
      }

#if 0
      prop->m_layer = layer;
      prop->m_currentSpriteIndex = frame;
      prop->m_animation.direction = direction;
#endif  // 0
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
                              std::vector<lfd::KeyFrame> keyFrames) {
  std::vector<renderer::Sprite> sprites;

  auto texture = createIndexTexture(m_sceneRenderer->renderer(), image);
  if (!texture) {
    return PropId::invalidValue();
  }

  RectI rect{image.left(), image.top(), image.width(), image.height()};
  sprites.emplace_back(texture, rect);

  auto propId = m_props.create(ResourceType::Image, name, m_delegate, m_film->frameCount(),
                               std::move(keyFrames), std::move(sprites));

  m_renderOrder.emplace_back(propId);

  return propId;
}

PropId Scene::insertAnimationProp(std::string_view name, const Animation& animation,
                                  std::vector<lfd::KeyFrame> keyFrames) {
  std::vector<renderer::Sprite> sprites;

  for (auto& image : animation.frames()) {
    auto texture = createIndexTexture(m_sceneRenderer->renderer(), image);
    if (!texture) {
      return PropId::invalidValue();
    }

    RectI rect{image.left(), image.top(), image.width(), image.height()};
    sprites.emplace_back(texture, rect);
  }

  auto propId = m_props.create(ResourceType::Animation, name, m_delegate, m_film->frameCount(),
                               std::move(keyFrames), std::move(sprites));

  m_renderOrder.emplace_back(propId);

  return propId;
}

void Scene::processFilm() {
  m_frameCount = m_film->frameCount();

  for (auto& block : m_film->blocks()) {
    spdlog::info("Processing block: {}", block.name);
    switch (block.type) {
      case lfd::BlockType::View: {
        processViewBlock(block);
        break;
      }

      case lfd::BlockType::Pltt: {
        processPaletteBlock(block);
        break;
      }

      case lfd::BlockType::Delt: {
        processImageBlock(block);
        break;
      }

      case lfd::BlockType::Anim: {
        processAnimationBlock(block);
        break;
      }

      case lfd::BlockType::End:
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

void Scene::processViewBlock(const lfd::Film::Block& block) {
  for (auto& chunk : block.keyFrames) {
    switch (chunk.opCode) {
      case lfd::OpCode::Time:
        spdlog::info("  - {} :: {}", opCodeToString(chunk.opCode), chunk.variables[0]);
        break;

      case lfd::OpCode::Transition:
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

void Scene::processPaletteBlock(const lfd::Film::Block& block) {
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

  for (auto& chunk : block.keyFrames) {
    spdlog::info("chunk: {}", opCodeToString(chunk.opCode));
  }

  applyPalette(*palette);
}

void Scene::processImageBlock(const lfd::Film::Block& block) {
  //  if (block.name != "cursors") {
  //    return;
  //  }
  insertImage(block.name, block.keyFrames);
}

void Scene::processAnimationBlock(const lfd::Film::Block& block) {
  //  if (block.name != "cursors") {
  //    return;
  //  }
  insertAnimation(block.name, block.keyFrames);
}

void Scene::advanceToNextFrame() {
  if (m_currentFrame >= m_frameCount) {
    if (m_delegate) {
      m_delegate->onSceneLastFramePlayed();
    }

    return;
  }

  // ++m_currentFrame;

  for (auto& propId : m_renderOrder) {
    auto prop = m_props.getData(propId);
    if (prop) {
      prop->sceneTick(m_currentFrame);
    }
  }
}

}  // namespace game
