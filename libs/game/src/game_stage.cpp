#include "game/game_stage.h"

namespace game {

GameStage::GameStage(std::shared_ptr<Resources> resources) : m_resources{std::move(resources)} {}

}  // namespace game
