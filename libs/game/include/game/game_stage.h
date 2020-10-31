#pragma once

#include <engine/stage.h>

#include "game/resources.h"

namespace game {

class GameStage : public engine::Stage {
public:
  explicit GameStage(std::shared_ptr<Resources> resources);

protected:
  std::shared_ptr<Resources> m_resources;
};

}  // namespace game
