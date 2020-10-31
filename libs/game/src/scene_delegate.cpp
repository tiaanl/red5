#include "game/scene_delegate.h"

namespace game {

SceneDelegate::~SceneDelegate() = default;

void SceneDelegate::onSceneReady() {}

void SceneDelegate::onSceneEvent(I16 event) {
  // spdlog::info("Unhandled event: {}", event);
}

void SceneDelegate::onSceneLastFramePlayed() {}

}  // namespace game
