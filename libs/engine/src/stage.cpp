#include "engine/stage.h"

namespace engine {

Stage::~Stage() = default;

void Stage::attachToEngine(renderer::Renderer* renderer) {
  m_renderer = renderer;
}

void Stage::detachFromEngine() {
  m_renderer = nullptr;
}

}  // namespace engine
