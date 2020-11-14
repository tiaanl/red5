#include "game/game_stage_state.h"

namespace game {

// static
std::shared_ptr<GameStageState> GameStageState::create(fs::path resourceRoot,
                                                       engine::Renderer* renderer) {
  std::shared_ptr<GameStageState> result(new GameStageState{std::move(resourceRoot), renderer});

  result->sceneRenderer.attachToRenderer(renderer);
  result->gameScreenRenderTarget = renderer->renderTargets().create({320, 200});

  return result;
}

GameStageState::~GameStageState() {
  m_renderer->renderTargets().destroy(gameScreenRenderTarget);

  sceneRenderer.detachFromRenderer();
}

GameStageState::GameStageState(fs::path resourceRoot, engine::Renderer* renderer)
  : resources{std::move(resourceRoot)}, m_renderer{renderer} {}

}  // namespace game
