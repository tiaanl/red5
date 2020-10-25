#include "engine/props.h"

#include <renderer/renderer.h>

#define TRACE_OP_CODES 0

namespace engine {

RenderItem::RenderItem(renderer::TextureId texture, const renderer::Rect& rect)
  : m_texture{texture}, m_rect{rect} {}

RenderItem::~RenderItem() {
  // TODO: Destroy the texture
}

bool RenderItem::render(renderer::Renderer* renderer, const renderer::Position& offset,
                        const renderer::Position& orientation) {
  renderer::Rect rect = m_rect;
  rect.position += offset;

#if 0
  U32 flip = SDL_FLIP_NONE;
  if (orientation.x > 0) {
    flip |= SDL_FLIP_HORIZONTAL;
  }
  if (orientation.y > 0) {
    flip |= SDL_FLIP_VERTICAL;
  }
#endif  // 0

  renderer->copyTexture(m_texture, rect);

  return true;
}

Prop::Prop(SceneDelegate* delegate, std::vector<Film::Chunk> chunks,
           std::vector<RenderItem> renderItems)
  : m_delegate{delegate}, m_chunks{std::move(chunks)}, m_renderItems{std::move(renderItems)} {}

void Prop::nextFrame(U32 sceneFrame) {
  if (sceneFrame == 0) {
    resetState();
  }

  updateState(sceneFrame);

  m_offset += m_movePerFrame;

  m_currentFrame += m_animation.direction;
  if (m_currentFrame < 0) {
    m_currentFrame = static_cast<I16>(m_renderItems.size()) - 1;
  } else if (m_currentFrame >= m_renderItems.size()) {
    m_currentFrame = 0;
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

void Prop::resetState() {
  m_visible = false;
  m_currentFrame = 0;
  m_layer = 0;
  m_offset = {0, 0};
  m_movePerFrame = {0, 0};
  m_animation.direction = 0;
  m_animation.frameRate = 0;
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

  m_currentFrame = frame;

  assert(x == 0);
}

void Prop::applyAnimation(I16 direction, I16 frameRate) {
#if TRACE_OP_CODES > 0
  spdlog::info("OpCode::Animation :: direction: {}, frameRate: {}", direction, frameRate);
#endif

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

void Prop::render(renderer::Renderer* renderer) {
  if (m_visible) {
    m_renderItems[m_currentFrame].render(renderer, m_offset, m_orientation);
  }
}

}  // namespace engine
