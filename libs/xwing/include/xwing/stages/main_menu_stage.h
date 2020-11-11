#pragma once

#include "game/single_scene_stage.h"

namespace xwing {

class MainMenuStage : public game::SingleSceneStage {
public:
  explicit MainMenuStage(std::shared_ptr<game::GameStageState> gameStageState);

protected:
  bool onLoad(game::Scene& scene) override;
  void onPropEnter(game::PropId propId) override;
  void onPropExit(game::PropId propId) override;
  void onPropClicked(game::PropId propId) override;

private:
  bool isDoor(std::string_view name) const;
};

}  // namespace xwing
