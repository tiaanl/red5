#include "game/scene_manager.h"

#include <engine/engine.h>
#include <game/scene_stage.h>

namespace game {

SceneManager::SceneManager(fs::path resourceRoot, engine::Engine* engine)
  : m_resourceRoot{std::move(resourceRoot)}, m_engine{engine} {}

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
    m_gameStageState = game::GameStageState::create(m_resourceRoot, &m_engine->renderer());
    m_gameStageState->resources.addResourceFile("xwing");
  }

  // Load in the required resources.
  for (const auto& resourceFile : sceneDescription.resourceFiles) {
    m_gameStageState->resources.addResourceFile({resourceFile});
  }

  m_engine->setStage(createStage(sceneDescription));
}

std::unique_ptr<engine::Stage> SceneManager::createStage(const SceneDescription& sceneDescription) {
  std::unique_ptr<SceneController> controller;
  if (sceneDescription.sceneControllerFactory) {
    controller = sceneDescription.sceneControllerFactory->create(this);
  }

  auto stage = std::make_unique<game::SceneStage>(this, m_gameStageState, std::move(controller),
                                                  sceneDescription.filmName);

  if (sceneDescription.autoPlay) {
    stage->setAutoPlay(sceneDescription.nextScene);
  }

  return stage;
}

}  // namespace game
