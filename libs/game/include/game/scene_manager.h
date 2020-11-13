#pragma once

#include "game_stage_state.h"
#include "scene_controller.h"

namespace engine {
class Engine;
}

namespace game {

class SceneManager;

struct SceneControllerFactory {
  virtual std::unique_ptr<SceneController> create(SceneManager* sceneManager) = 0;
};

template <typename SceneControllerType>
struct TypedSceneControllerFactory : SceneControllerFactory {
  std::unique_ptr<SceneController> create(SceneManager* sceneManager) override {
    return std::make_unique<SceneControllerType>(sceneManager);
  }
};

template <typename SceneControllerType>
std::unique_ptr<SceneControllerFactory> sceneControllerFactory() {
  return std::make_unique<TypedSceneControllerFactory<SceneControllerType>>();
}

class SceneManager {
public:
  explicit SceneManager(engine::Engine* engine);

  void registerScene(std::string_view name, std::vector<std::string> resourceFiles,
                     std::unique_ptr<SceneControllerFactory> sceneControllerFactory);

  void switchToScene(const std::string& name);

private:
  struct RegisteredScene {
    std::vector<std::string> resourceFiles;
    std::unique_ptr<SceneControllerFactory> sceneControllerFactory;
  };

  engine::Engine* m_engine;
  std::shared_ptr<game::GameStageState> m_gameStageState;
  std::unordered_map<std::string, RegisteredScene> m_scenes;
};

}  // namespace game
