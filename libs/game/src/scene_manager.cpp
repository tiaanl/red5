#include "game/scene_manager.h"

#include <engine/engine.h>
#include <game/scene_stage.h>

namespace game {

SceneManager::SceneManager(engine::Engine* engine) : m_engine{engine} {}

void SceneManager::registerScene(std::string_view name, std::vector<std::string> resourceFiles,
                                 std::unique_ptr<SceneControllerFactory> sceneControllerFactory) {
  RegisteredScene registeredScene{std::move(resourceFiles), std::move(sceneControllerFactory)};
  m_scenes.emplace(name, std::move(registeredScene));
}

void SceneManager::switchToScene(const std::string& name) {
  auto result = m_scenes.find(name);
  if (result == std::end(m_scenes)) {
    spdlog::error("No scene registered with name \"{}\"", name);
    return;
  }

  RegisteredScene& registeredScene = result->second;

  spdlog::info("Switching to scene: {}", name);

  if (!m_gameStageState) {
    m_gameStageState = game::GameStageState::create(&m_engine->renderer());
  }

  auto controller = registeredScene.sceneControllerFactory->create(this);
  auto stage = std::make_unique<game::SceneStage>(m_gameStageState, std::move(controller));
  m_engine->setStage(std::move(stage));
}

}  // namespace game
