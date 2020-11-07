#include "engine/stage.h"

namespace engine {

Stage::~Stage() = default;

bool Stage::attachToEngine(Renderer* renderer) {
  m_renderer = renderer;

  return true;
}

void Stage::detachFromEngine() {
  m_renderer = nullptr;
}

void Stage::onStageResized(I32 width, I32 height) {}

void Stage::onMouseMoved(I32 x, I32 y) {}

}  // namespace engine
