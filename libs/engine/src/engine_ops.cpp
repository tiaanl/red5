#include "engine/engine_ops.h"

#include "engine/stage.h"

namespace engine {

void EngineOps::switchStage(std::unique_ptr<Stage> stage) {
  assert(!m_switchToStage);
  m_switchToStage = std::move(stage);
}

}  // namespace engine
