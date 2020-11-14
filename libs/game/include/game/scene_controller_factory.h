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

struct FunctionSceneControllerFactory : SceneControllerFactory {
  using FunctionType = std::function<std::unique_ptr<SceneController>(SceneManager*)>;

  explicit FunctionSceneControllerFactory(FunctionType function) : function{std::move(function)} {}

  std::unique_ptr<SceneController> create(SceneManager* sceneManager) override {
    return std::invoke(function, sceneManager);
  }

  FunctionType function;
};

template <typename SceneControllerType>
std::unique_ptr<SceneControllerFactory> sceneControllerFactory() {
  return std::make_unique<TypedSceneControllerFactory<SceneControllerType>>();
}

}  // namespace game
