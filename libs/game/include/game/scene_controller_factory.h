#pragma once

#include "game/scene_controller.h"

namespace game {

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

}  // namespace game
