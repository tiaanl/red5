#pragma once

namespace engine {

class SceneDelegate {
public:
  virtual ~SceneDelegate();

  virtual void onSceneReady();
  virtual void onSceneEvent(I16 event);
  virtual void onSceneLastFramePlayed();
};

}  // namespace engine
