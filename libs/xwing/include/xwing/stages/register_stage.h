#pragma once

#include <base/platform.h>
#include <game/single_scene_stage.h>

namespace xwing {

class RegisterStage : public game::SingleSceneStage {
public:
  explicit RegisterStage(std::shared_ptr<game::Resources> resources);
  ~RegisterStage() override;
};

}  // namespace xwing
