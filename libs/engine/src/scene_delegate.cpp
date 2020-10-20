#include "engine/scene_delegate.h"

namespace engine {

SceneDelegate::~SceneDelegate() = default;

void SceneDelegate::onSceneReady() {}

void SceneDelegate::onSceneEvent(I16 event) {
  // spdlog::info("Unhandled event: {}", event);
}

void SceneDelegate::onSceneLastFramePlayed() {}

}  // namespace engine
