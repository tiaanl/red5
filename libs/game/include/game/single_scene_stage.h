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
  bool onLoad() override final;
  void onMouseMoved(const PositionI& mousePosition) override;
  void onUpdate(U32 millis) override;

  // Override: GameStage
  void onRenderGameScreen() override final;
#if DEBUG_UI > 0
  void onRenderDebugInfo() override final;
#endif  // DEBUG_UI < 0

  // Override: SceneDelegate
  void onSceneReady() override;
  void onSceneEvent(I16 event) override;
  void onSceneLastFramePlayed() override;

protected:
  virtual bool onLoad(Scene& scene);

  virtual void onPropEnter(PropId propId);
  virtual void onPropExit(PropId propId);

  std::unique_ptr<Scene> m_scene;

private:
  PositionI windowCoordToSceneCoord(const PositionI& windowCoord);
  void setPropUnderMouse(PropId propId);

  std::string m_filmName;

  PropId m_mouseCursor;

  PropId m_propUnderMouse;
};

}  // namespace game
