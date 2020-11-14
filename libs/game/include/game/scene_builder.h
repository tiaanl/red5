#pragma once

#include <string>
#include <vector>

#include "game/scene_controller_factory.h"
#include "game/scene_description.h"

namespace game {

class SceneBuilder {
  NO_COPY(SceneBuilder);

public:
  explicit SceneBuilder(std::string_view filmName) : m_filmName{filmName} {}

  SceneBuilder& resourceFile(std::string_view resourceFile) {
    m_resourceFiles.emplace_back(resourceFile);

    return *this;
  }

  template <typename SceneControllerType>
  SceneBuilder& sceneController() {
    m_sceneControllerFactory = sceneControllerFactory<SceneControllerType>();

    return *this;
  }

  SceneBuilder& sceneController(FunctionSceneControllerFactory::FunctionType factoryFunction) {
    m_sceneControllerFactory =
        std::make_unique<FunctionSceneControllerFactory>(std::move(factoryFunction));

    return *this;
  }

  SceneBuilder& autoPlay(bool autoPlay = true, std::optional<std::string_view> nextScene = {}) {
    m_autoPlay = autoPlay;
    m_nextScene = nextScene;

    return *this;
  }

  SceneDescription build() {
    SceneDescription result;
    result.filmName = std::move(m_filmName);
    result.resourceFiles = std::move(m_resourceFiles);
    result.sceneControllerFactory = std::move(m_sceneControllerFactory);
    result.autoPlay = m_autoPlay;
    result.nextScene = m_nextScene;

    return result;
  }

private:
  std::string m_filmName;
  std::vector<std::string> m_resourceFiles;
  std::unique_ptr<SceneControllerFactory> m_sceneControllerFactory;
  bool m_autoPlay = false;
  std::optional<std::string> m_nextScene;
};

}  // namespace game
