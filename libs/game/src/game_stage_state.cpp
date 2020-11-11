#include "game/game_stage_state.h"

namespace game {

// static
std::shared_ptr<GameStageState> GameStageState::create(engine::Renderer* renderer) {
  auto result = std::make_shared<GameStageState>();

  result->sceneRenderer.attachToRenderer(renderer);

  return result;
}

GameStageState::GameStageState() = default;

GameStageState::~GameStageState() {
  sceneRenderer.detachFromRenderer();
}

}  // namespace game
