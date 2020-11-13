#pragma once

#include <engine/stage.h>

#include "game/game_stage_state.h"
#include "game/scene.h"
#include "game/scene_controller.h"

namespace game {

class SceneStage : public engine::Stage {
public:
  explicit SceneStage(std::shared_ptr<GameStageState> gameStageState,
                      std::unique_ptr<SceneController> controller);
  ~SceneStage() override;

  // Override: engine::Stage
  bool onAttachedToEngine(engine::Renderer* renderer) override;
  void onDetachFromEngine() override;
  void onStageResized(I32 width, I32 height) override;
  void onMouseMoved(const PositionI& mousePosition) override;
  void onMousePressed(const PositionI& mousePosition, U8 buttons) override;
  void onMouseReleased(const PositionI& mousePosition, U8 buttons) override;
  void onUpdate(U32 millis) override;
  void onRender() override;

private:
  PositionI windowCoordToSceneCoord(const PositionI& windowCoord);
  void setPropUnderMouse(PropId propId);

  std::shared_ptr<GameStageState> m_gameStageState;
  std::unique_ptr<SceneController> m_controller;

  RectI m_gameScreenRect;
  std::unique_ptr<Scene> m_scene;

  PropId m_propUnderMouse;
  PropId m_propUnderMousePress;
};

}  // namespace game
