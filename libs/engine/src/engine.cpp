#include "engine/engine.h"

namespace engine {

Engine::Engine(Resources* resources, SDL_Renderer* renderer)
  : m_resources{resources}, m_renderer{renderer} {}

void Engine::setStage(std::unique_ptr<Stage> stage) {
  spdlog::info("Setting stage");

  m_currentStage = std::move(stage);
  m_currentStage->onReady();
}

void Engine::update(U32 ticks) {
  if (m_currentStage) {
    m_currentStage->onUpdate(ticks);
  }
}

void Engine::render() {
  if (m_currentStage) {
    m_currentStage->onRender();
  }
}

}  // namespace engine
