#include "game/game_stage_state.h"

namespace game {

// static
std::shared_ptr<GameStageState> GameStageState::create(engine::Renderer* renderer) {
  std::shared_ptr<GameStageState> result(new GameStageState{renderer});

  result->sceneRenderer.attachToRenderer(renderer);
  result->gameScreenRenderTarget = renderer->renderTargets().create({320, 200});

  return result;
}

GameStageState::~GameStageState() {
  m_renderer->renderTargets().destroy(gameScreenRenderTarget);

  sceneRenderer.detachFromRenderer();
}

GameStageState::GameStageState(engine::Renderer* renderer) : m_renderer{renderer} {}

}  // namespace game
