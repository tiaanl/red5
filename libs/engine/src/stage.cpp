#include "engine/stage.h"

namespace engine {

Stage::~Stage() = default;

void Stage::attachToEngine(Resources* resources, SDL_Renderer* renderer) {
  m_resources = resources;
  m_renderer = renderer;
}

void Stage::detachFromEngine() {
  m_resources = nullptr;
  m_renderer = nullptr;
}

}  // namespace engine
