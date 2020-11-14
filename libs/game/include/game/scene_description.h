#pragma once

#include "game/scene_controller.h"

namespace game {

class SceneDescription {
public:
  SceneDescription() = default;

  NO_COPY(SceneDescription);
  DEFAULT_MOVE(SceneDescription);

  const auto& resourceFiles() const {
    return m_resourceFiles;
  }

  void setResourceFiles(std::vector<std::string> resourceFiles) {
    m_resourceFiles = std::move(resourceFiles);
  }

  const auto& sceneControllerFactory() const {
    return m_sceneControllerFactory;
  }

  void setSceneControllerFactory(std::unique_ptr<SceneControllerFactory> sceneControllerFactory) {
    m_sceneControllerFactory = std::move(sceneControllerFactory);
  }

private:
  std::vector<std::string> m_resourceFiles;
  std::unique_ptr<SceneControllerFactory> m_sceneControllerFactory;
};

}  // namespace game
