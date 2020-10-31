#include "xwing/stages/register_stage.h"

namespace xwing {

RegisterStage::RegisterStage(std::shared_ptr<game::Resources> resources)
  : SingleSceneStage{std::move(resources), "pilot"} {}

RegisterStage::~RegisterStage() = default;

}  // namespace xwing
