#pragma once

#include "game/game_stage_state.h"
#include "game/scene_controller.h"
#include "game/scene_controller_factory.h"
#include "game/scene_description.h"

namespace engine {
class Engine;
class Stage;
}  // namespace engine

namespace game {

class SceneManager {
public:
  explicit SceneManager(engine::Engine* engine);

  void registerScene(std::string_view name, SceneDescription sceneDescription);

  void switchToScene(const std::string& name);

private:
  std::unique_ptr<engine::Stage> createStage(const SceneDescription& sceneDescription);

  engine::Engine* m_engine;
  std::shared_ptr<game::GameStageState> m_gameStageState;
  std::unordered_map<std::string, SceneDescription> m_scenes;
};

}  // namespace game
