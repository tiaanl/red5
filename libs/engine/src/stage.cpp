#include "engine/stage.h"

#include "engine/engine.h"

namespace engine {

Stage::Stage() = default;

Stage::~Stage() = default;

void Stage::onStageResized(I32 width, I32 height) {}

void Stage::onMouseMoved(const PositionI&) {}

void Stage::onMousePressed(const PositionI& mousePosition, U8 buttons) {}

void Stage::onMouseReleased(const PositionI& mousePosition, U8 buttons) {}

bool Stage::attachToEngine(EngineOps* engineOps, Renderer* renderer) {
  m_engineOps = engineOps;
  m_renderer = renderer;

  if (!onAttachedToEngine(renderer)) {
    return false;
  }

  return true;
}
void Stage::detachFromEngine() {
  onDetachFromEngine();

  m_engineOps = nullptr;
  m_renderer = nullptr;
}

}  // namespace engine
