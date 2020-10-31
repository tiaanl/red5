#pragma once

namespace game {

class SceneDelegate {
public:
  virtual ~SceneDelegate();

  virtual void onSceneReady();
  virtual void onSceneEvent(I16 event);
  virtual void onSceneLastFramePlayed();
};

}  // namespace game
