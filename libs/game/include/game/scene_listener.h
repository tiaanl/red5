#pragma once

namespace game {

class SceneListener {
public:
  virtual ~SceneListener() = default;

  virtual void onSceneEvent(I16 event) = 0;
  virtual void onSceneLastFramePlayed() = 0;
};

}  // namespace game
