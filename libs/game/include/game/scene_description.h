#pragma once

#include "game/scene_controller.h"

namespace game {

struct SceneDescription {
  std::string filmName;
  std::vector<std::string> resourceFiles;
  std::unique_ptr<SceneControllerFactory> sceneControllerFactory;
  bool autoPlay = false;
  std::optional<std::string> nextScene;
};

}  // namespace game
