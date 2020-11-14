#include "game/scene_manager.h"

#include <engine/engine.h>
#include <game/scene_stage.h>

namespace game {

SceneManager::SceneManager(engine::Engine* engine) : m_engine{engine} {}

void SceneManager::registerScene(std::string_view name, SceneDescription sceneDescription) {
  m_scenes.emplace(name, std::move(sceneDescription));
}

void SceneManager::switchToScene(const std::string& name) {
  auto result = m_scenes.find(name);
  if (result == std::end(m_scenes)) {
    spdlog::error("No scene registered with name \"{}\"", name);
    return;
  }

  auto& sceneDescription = result->second;

  spdlog::info("Switching to scene: {}", name);

  if (!m_gameStageState) {
    m_gameStageState = game::GameStageState::create(&m_engine->renderer());
    m_gameStageState->resources.addResourceFile({R"(C:\XWING\RESOURCE\XWING.LFD)"});
  }

  // Load in the required resources.
  for (const auto& resourceFile : sceneDescription.resourceFiles()) {
    m_gameStageState->resources.addResourceFile({resourceFile});
  }

  m_engine->setStage(createStage(sceneDescription));
}

std::unique_ptr<engine::Stage> SceneManager::createStage(const SceneDescription& sceneDescription) {
  auto controller = sceneDescription.sceneControllerFactory()->create(this);
  return std::make_unique<game::SceneStage>(m_gameStageState, std::move(controller));
}

}  // namespace game
