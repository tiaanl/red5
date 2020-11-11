#pragma once

#include "game/scene_renderer.h"
#include "resources.h"

namespace game {

struct GameStageState {
  static std::shared_ptr<GameStageState> create(engine::Renderer* renderer);

  GameStageState();
  ~GameStageState();

  Resources resources;
  SceneRenderer sceneRenderer;
};

}  // namespace game
