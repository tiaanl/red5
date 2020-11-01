#include "game/props.h"

#include <game/scene_renderer.h>

#define TRACE_OP_CODES 0

namespace game {

Prop::Prop(ResourceType type, std::string_view name, SceneDelegate* delegate,
           std::vector<Film::Chunk> chunks, std::vector<renderer::Sprite> sprites,
           std::vector<renderer::Sprite> indexSprites)
  : m_resourceType{type},
    m_name{name},
    m_delegate{delegate},
    m_chunks{std::move(chunks)},
    m_sprites{std::move(sprites)},
    m_indexSprites{std::move(indexSprites)} {}

void Prop::setSpriteIndex(I16 currentFrame) {
  m_currentSpriteIndex = currentFrame;
}

void Prop::setLayer(I16 layer) {
  m_layer = layer;
}

void Prop::setOffset(const renderer::Position& offset) {
  m_offset = offset;
}

void Prop::sceneTick(I32 sceneFrame) {
  updateState(sceneFrame);

  m_currentSpriteIndex += m_animation.direction;
  if (m_currentSpriteIndex >= m_sprites.size()) {
    m_currentSpriteIndex = static_cast<I16>(m_sprites.size() - 1);
  } else if (m_currentSpriteIndex < 0) {
    m_currentSpriteIndex = 0;
  }

  m_offset += m_movePerFrame;
}

void Prop::render(SceneRenderer* renderer) {
  if (m_visible) {
    // auto sprite = m_sprites[m_currentSpriteIndex];
    auto sprite = m_indexSprites[m_currentSpriteIndex];
    sprite.setPosition(sprite.position() + m_offset);
    renderer->render(sprite);
  }
}

void Prop::updateState(U32 frame) {
  // Find the first chunk that has the same time as `frame`.
  auto it =
      std::find_if(std::begin(m_chunks), std::end(m_chunks), [&frame](const Film::Chunk& chunk) {
        return chunk.opCode == OpCode::Time && chunk.variables[0] == frame;
      });

  if (it == std::end(m_chunks)) {
    return;
  }

#if TRACE_OP_CODES > 0
  spdlog::info("Found first time at: {}", frame);
#endif

  for (++it; it != std::end(m_chunks); ++it) {
    if (it->opCode == OpCode::Time || it->opCode == OpCode::End) {
      break;
    }

    switch (it->opCode) {
      case OpCode::Move:
        assert(it->variables.size() == 4);
        applyMove(it->variables[0], it->variables[1], it->variables[2], it->variables[3]);
        break;

      case OpCode::Speed:
        assert(it->variables.size() == 4);
        applySpeed(it->variables[0], it->variables[1], it->variables[2], it->variables[3]);
        break;

      case OpCode::Layer:
        assert(it->variables.size() == 1);
        applyLayer(it->variables[0]);
        break;

      case OpCode::Frame:
        assert(it->variables.size() == 2);
        applyFrame(it->variables[0], it->variables[1]);
        break;

      case OpCode::Animation:
        assert(it->variables.size() == 2);
        applyAnimation(it->variables[0], it->variables[1]);
        break;

      case OpCode::Event:
        assert(it->variables.size() == 1);
        applyEvent(it->variables[0]);
        break;

      case OpCode::Window:
        assert(it->variables.size() == 4);
        applyWindow(it->variables[0], it->variables[1], it->variables[2], it->variables[3]);
        break;

      case OpCode::Shift:
        assert(it->variables.size() == 4);
        applyShift(it->variables[0], it->variables[1], it->variables[2], it->variables[3]);
        break;

      case OpCode::Display:
        assert(it->variables.size() == 1);
        applyDisplay(it->variables[0]);
        break;

      case OpCode::Orientation:
        assert(it->variables.size() == 2);
        applyOrientation(it->variables[0], it->variables[1]);
        break;

      default: {
        spdlog::warn("OpCode not handled: {}", opCodeToString(it->opCode));
        break;
      }
    }
  }
}

void Prop::applyMove(I16 x, I16 y, I16 xx, I16 yy) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Move :: x: {}, y: {}, xx: {}, yy: {}", x, y, xx, yy);
#endif

  m_offset.left += x;
  m_offset.top += y;

  assert(xx == 0);
  assert(yy == 0);
}

void Prop::applySpeed(I16 x, I16 y, I16 xx, I16 yy) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Speed :: x: {}, y: {}, xx: {}, yy: {}", x, y, xx, yy);
#endif

  m_movePerFrame.left = x;
  m_movePerFrame.top = y;

  assert(xx == 0);
  assert(yy == 0);
}

void Prop::applyLayer(I16 layer) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Layer :: layer: {}", layer);
#endif

  m_layer = layer;
}

void Prop::applyFrame(I16 frame, I16 x) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Frame :: frame: {}, x: {}", frame, x);
#endif

  m_currentSpriteIndex = frame;

  assert(x == 0);
}

void Prop::applyAnimation(I16 direction, I16 frameRate) {
  // #if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Animation :: direction: {}, frameRate: {}", direction, frameRate);
  // #endif

  m_animation.direction = direction;
  m_animation.frameRate = frameRate;
}

void Prop::applyEvent(I16 event) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Event :: event: {}", event);
#endif

  m_delegate->onSceneEvent(event);
}

void Prop::applyWindow(I16 x, I16 y, I16 w, I16 h) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Window :: x: {}, y: {}, w: {}, h: {}", x, y, w, h);
#endif

  (void)x;
  (void)y;
  (void)w;
  (void)h;
}

void Prop::applyShift(I16 x, I16 y, I16 xx, I16 yy) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Shift :: x: {}, y: {}, xx: {}, yy: {}", x, y, xx, yy);
#endif

  m_offset.left = x;
  m_offset.top = y;

  assert(xx == 0);
  assert(yy == 0);
}

void Prop::applyDisplay(I16 visible) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Display :: visible: {}", visible);
#endif

  m_visible = visible != 0;
}

void Prop::applyOrientation(I16 x, I16 y) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Orientation :: x: {}, y: {}", x, y);
#endif

  m_orientation.left = x;
  m_orientation.top = y;
}

renderer::ResourceContainer<Prop>::Identifier PropContainer::create(
    ResourceType type, std::string_view name, SceneDelegate* delegate,
    std::vector<Film::Chunk> chunks, std::vector<renderer::Sprite> sprites,
    std::vector<renderer::Sprite> indexSprites) {
  return emplaceData(type, name, delegate, std::move(chunks), std::move(sprites),
                     std::move(indexSprites));
}

}  // namespace game
