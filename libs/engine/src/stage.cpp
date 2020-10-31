#include "engine/stage.h"

namespace engine {

Stage::~Stage() = default;

void Stage::attachToEngine(renderer::Renderer* renderer) {
  m_renderer = renderer;
}

void Stage::detachFromEngine() {
  m_renderer = nullptr;
}

void Stage::onStageResized(I32 width, I32 height) {}

void Stage::onMouseMoved(I32 x, I32 y) {}

}  // namespace engine
