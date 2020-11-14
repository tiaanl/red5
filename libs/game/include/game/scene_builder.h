#pragma once

#include <string>
#include <vector>

#include "game/scene_controller_factory.h"
#include "game/scene_description.h"

namespace game {

class SceneBuilder {
  NO_COPY(SceneBuilder);

public:
  SceneBuilder() = default;

  SceneBuilder& resourceFile(std::string_view resourceFile) {
    m_resourceFiles.emplace_back(resourceFile);
    return *this;
  }

  template <typename SceneControllerType>
  SceneBuilder& sceneController() {
    m_sceneControllerFactory = sceneControllerFactory<SceneControllerType>();
    return *this;
  }

  SceneDescription build() {
    SceneDescription result;
    result.setResourceFiles(std::move(m_resourceFiles));
    result.setSceneControllerFactory(std::move(m_sceneControllerFactory));
    return result;
  }

private:
  std::vector<std::string> m_resourceFiles;
  std::unique_ptr<SceneControllerFactory> m_sceneControllerFactory;
};

}  // namespace game
