#include "engine/engine.h"

#include <engine/true_type_font.h>

namespace engine {

Engine::Engine(Resources* resources, SDL_Renderer* renderer)
  : m_resources{resources}, m_renderer{renderer}, m_ttf(20) {}

void Engine::setStage(std::unique_ptr<Stage> stage) {
  spdlog::info("Setting stage");

  m_ttf.loadFromFont(m_renderer);

  if (m_currentStage) {
    m_currentStage->detachFromEngine();
  }

  m_currentStage = std::move(stage);

  m_currentStage->attachToEngine(m_resources, m_renderer);
  m_currentStage->onReady();
}

void Engine::update(U32 ticks) {
  if (m_currentStage) {
    m_currentStage->onUpdate(ticks);
  }
}

void Engine::renderGameScreen() {
  if (m_currentStage) {
    m_currentStage->onRender();
  }
}

void Engine::renderOverlay() {
  m_ttf.renderText(m_renderer, {10, 10}, "test");
}

}  // namespace engine
