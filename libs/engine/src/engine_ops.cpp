#include "engine/engine_ops.h"

#include "engine/stage.h"

namespace engine {

void EngineOps::switchStage(std::unique_ptr<Stage> stage) {
  m_switchToStage = std::move(stage);
}

}  // namespace engine
