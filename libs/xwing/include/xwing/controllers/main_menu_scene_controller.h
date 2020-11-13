#pragma once

#include "game/scene_controller.h"

namespace xwing {

class MainMenuSceneController : public game::SceneController {
public:
  explicit MainMenuSceneController(game::SceneManager* sceneManager);

  // Override: game::SceneController
  bool setUpScene(game::Scene& scene, game::Resources& resources) override;
  void onPropEnter(game::Scene& scene, game::PropId propId) override;
  void onPropExit(game::Scene& scene, game::PropId propId) override;
  void onPropClicked(game::Scene& scene, game::PropId propId) override;
  void onUpdate(game::Scene& scene, U32 millis) override;
};

}  // namespace xwing
