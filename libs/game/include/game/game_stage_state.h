#pragma once

#include <filesystem>

#include "game/resources.h"
#include "game/scene_renderer.h"

namespace fs = std::filesystem;

namespace game {

struct GameStageState {
  static std::shared_ptr<GameStageState> create(fs::path resourceRoot, engine::Renderer* renderer);

  ~GameStageState();

  Resources resources;
  SceneRenderer sceneRenderer;
  engine::RenderTargetId gameScreenRenderTarget;

private:
  explicit GameStageState(fs::path resourceRoot, engine::Renderer* renderer);

  engine::Renderer* m_renderer;
};

}  // namespace game
