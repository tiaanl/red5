#pragma once

#include <base/platform.h>
#include <game/single_scene_stage.h>

namespace xwing {

class RegisterStage : public game::SingleSceneStage {
public:
  explicit RegisterStage(std::shared_ptr<game::Resources> resources);
  ~RegisterStage() override;

public:
  void onUpdate(U32 millis) override;

protected:
  bool onLoad(game::Scene& scene) override;
  void onPropEnter(game::PropId propId) override;
  void onPropExit(game::PropId propId) override;
  void onPropClicked(game::PropId propId) override;

private:
  bool initRobot(game::Scene& scene);

  game::PropId m_doorId;

  game::PropId m_robotBodyId;
  game::PropId m_robotHeadId;
  game::PropId m_robotArmLeftId;
  game::PropId m_robotArmRightId;

  U32 m_robotMillis = 0;
};

}  // namespace xwing
