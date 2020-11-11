#pragma once

#include <engine/stage.h>

#include "game/game_stage_state.h"
#include "game/resources.h"

namespace game {

class GameStage : public engine::Stage {
public:
  explicit GameStage(std::shared_ptr<GameStageState> gameSharedState);

  virtual void onRenderGameScreen() = 0;
#if DEBUG_UI > 0
  virtual void onRenderDebugInfo();
#endif  // DEBUG_UI > 0

  // Override: engine::Stage
  bool onLoad() override;
  void onStageResized(I32 width, I32 height) override;
  void onUpdate(U32 millis) override;
  void onRender() override final;

protected:
  std::shared_ptr<GameStageState> m_gameStageState;

  RectI m_gameScreenRect{0, 0, 0, 0};

private:
  engine::Renderer* m_attachedRenderer = nullptr;
  engine::RenderTargetId m_gameScreen;
};

template <typename StageType, typename... Args>
std::unique_ptr<GameStage> createGameStage(std::shared_ptr<GameStageState> gameStageState,
                                           Args&&... args) {
  return std::make_unique<StageType>(gameStageState, std::forward<Args>(args)...);
}

}  // namespace game
