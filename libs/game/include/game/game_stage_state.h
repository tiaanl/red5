#pragma once

#include "game/scene_renderer.h"
#include "resources.h"

namespace game {

struct GameStageState {
  static std::shared_ptr<GameStageState> create(engine::Renderer* renderer);

  ~GameStageState();

  Resources resources;
  SceneRenderer sceneRenderer;
  engine::RenderTargetId  gameScreenRenderTarget;

private:
  explicit GameStageState(engine::Renderer* renderer);

  engine::Renderer* m_renderer;
};

}  // namespace game
