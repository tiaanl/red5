#include "game/scene_controller.h"

namespace game {

SceneController::SceneController(SceneManager* sceneManager) : m_sceneManager{sceneManager} {}

void SceneController::onPropEnter(Scene& scene, PropId propId) {}

void SceneController::onPropExit(Scene& scene, PropId propId) {}

void SceneController::onPropClicked(Scene& scene, PropId propId) {}

void SceneController::onUpdate(Scene& scene, U32 millis) {}

SceneManager& SceneController::sceneManager() {
  assert(m_sceneManager);
  return *m_sceneManager;
}

}  // namespace game
