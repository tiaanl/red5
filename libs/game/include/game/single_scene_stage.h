#pragma once

#include <engine/stage.h>

#include "game/game_stage.h"
#include "game/scene.h"

namespace game {

class SingleSceneStage : public GameStage, public SceneDelegate {
public:
  SingleSceneStage(std::shared_ptr<Resources> resources, std::string_view filmName);

  bool addResourceFile(const ResourceFile& resourceFile);

  // Override: engine::Stage
  bool onLoad() override;
  void onMouseMoved(I32 x, I32 y) override;
  void onUpdate(U32 millis) override;

public:
  // Override: GameStage
  void onRenderGameScreen() override;
  void onRenderDebugInfo() override;

  // Override: SceneDelegate
  void onSceneReady() override;
  void onSceneEvent(I16 event) override;
  void onSceneLastFramePlayed() override;

protected:
  // Override: engine::Stage
  bool attachToEngine(engine::Renderer* renderer) override;

private:
  std::string m_filmName;

  std::unique_ptr<Scene> m_scene;
  PropId m_mouseCursor;

  PropId m_propUnderMouse;
};

}  // namespace game
