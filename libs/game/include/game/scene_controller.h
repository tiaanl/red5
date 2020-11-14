#pragma once

#include "game/resources.h"
#include "game/scene.h"

namespace game {

class SceneManager;

class SceneController {
public:
  explicit SceneController(SceneManager* sceneManager);

  virtual bool setUpScene(Scene& scene) = 0;

  virtual void onPropEnter(Scene& scene, PropId propId);
  virtual void onPropExit(Scene& scene, PropId propId);
  virtual void onPropClicked(Scene& scene, PropId propId);

  virtual void onUpdate(Scene& scene, U32 millis);

protected:
  SceneManager& sceneManager();

private:
  SceneManager* m_sceneManager;
};

}  // namespace game
