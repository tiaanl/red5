#pragma once

#include "game/resources.h"
#include "game/scene.h"

namespace game {

class SceneController {
public:
  virtual bool setUpScene(Scene& scene, Resources& resources) = 0;

  virtual void onPropEnter(Scene& scene, PropId propId);
  virtual void onPropExit(Scene& scene, PropId propId);
  virtual void onPropClicked(Scene& scene, PropId propId);

  virtual void onUpdate(Scene& scene, U32 millis);
};

}  // namespace game
