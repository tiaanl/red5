#pragma once

#include "game/props.h"
#include "game/scene_controller.h"

namespace xwing {

class RegisterSceneController : public game::SceneController {
public:
  RegisterSceneController(game::SceneManager* sceneManager);

  // Override: game::SceneController
  bool setUpScene(game::Scene& scene, game::Resources& resources) override;
  void onPropEnter(game::Scene& scene, game::PropId propId) override;
  void onPropExit(game::Scene& scene, game::PropId propId) override;
  void onPropClicked(game::Scene& scene, game::PropId propId) override;
  void onUpdate(game::Scene& scene, U32 millis) override;

private:
  game::PropId m_doorId;

  struct Robot {
    game::PropId bodyId;
    game::PropId headId;
    game::PropId armLeftId;
    game::PropId armRightId;

    U32 millisSinceLastUpdate = 0;

    bool init(game::Scene& scene);
    void update(game::Scene& scene, U32 millis);
  } m_robot;
};

}  // namespace xwing
