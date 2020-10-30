#include "engine/stage.h"

#include <renderer/renderer.h>
#include <renderer/sprite_renderer.h>

namespace engine {

Stage::~Stage() = default;

void Stage::attachToEngine(Resources* resources, renderer::SpriteRenderer* renderer) {
  m_resources = resources;
  m_renderer = renderer;
}

void Stage::detachFromEngine() {
  m_resources = nullptr;
  m_renderer = nullptr;
}

}  // namespace engine
