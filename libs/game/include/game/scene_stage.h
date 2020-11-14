#pragma once

#include <engine/stage.h>

#include "game/game_stage_state.h"
#include "game/scene.h"
#include "game/scene_controller.h"

namespace game {

class SceneManager;

class SceneStage : public engine::Stage, public SceneListener {
public:
  explicit SceneStage(SceneManager* sceneManager, std::shared_ptr<GameStageState> gameStageState,
                      std::unique_ptr<SceneController> controller, std::string_view filmName);
  ~SceneStage() override;

  void setAutoPlay(std::optional<std::string_view> nextScene = {});

  // Override: engine::Stage
  bool onAttachedToEngine(engine::Renderer* renderer) override;
  void onDetachFromEngine() override;
  void onStageResized(I32 width, I32 height) override;
  void onMouseMoved(const PositionI& mousePosition) override;
  void onMousePressed(const PositionI& mousePosition, U8 buttons) override;
  void onMouseReleased(const PositionI& mousePosition, U8 buttons) override;
  void onUpdate(U32 millis) override;
  void onRender() override;

  // Override: SceneListener
  void onSceneEvent(I16 event) override;
  void onSceneLastFramePlayed() override;

private:
  PositionI windowCoordToSceneCoord(const PositionI& windowCoord);
  void setPropUnderMouse(PropId propId);

  SceneManager* m_sceneManager;
  std::shared_ptr<GameStageState> m_gameStageState;
  std::unique_ptr<SceneController> m_controller;
  std::string m_filmName;

  bool m_autoPlay = false;
  std::optional<std::string> m_nextScene;

  RectI m_gameScreenRect;
  std::unique_ptr<Scene> m_scene;

  PropId m_propUnderMouse;
  PropId m_propUnderMousePress;
};

}  // namespace game
