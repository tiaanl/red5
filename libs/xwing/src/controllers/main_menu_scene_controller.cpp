#include "xwing/controllers/main_menu_scene_controller.h"

#include <game/scene_manager.h>

namespace xwing {

namespace {

bool isDoor(std::string_view name) {
  if (name == "middoor") {
    return true;
  }

  if (name == "lhdoor") {
    return true;
  }

  if (name == "toddoor") {
    return true;
  }

  if (name == "lwr-lhdr") {
    return true;
  }

  if (name == "lwr-rhdr") {
    return true;
  }

  if (name == "pilotdr") {
    return true;
  }

  return false;
}

}  // namespace

MainMenuSceneController::MainMenuSceneController(game::SceneManager* sceneManager)
  : SceneController{sceneManager} {}

bool MainMenuSceneController::setUpScene(game::Scene& scene) {
  scene.prop(scene.propId("middoor"))->setAnimate(false);
  scene.prop(scene.propId("lhdoor"))->setAnimate(false);
  scene.prop(scene.propId("toddoor"))->setAnimate(false);
  scene.prop(scene.propId("lwr-lhdr"))->setAnimate(false);
  scene.prop(scene.propId("lwr-rhdr"))->setAnimate(false);
  scene.prop(scene.propId("pilotdr"))->setAnimate(false);

  return true;
}

void MainMenuSceneController::onPropEnter(game::Scene& scene, game::PropId propId) {
  SceneController::onPropEnter(scene, propId);

  auto* prop = scene.prop(propId);
  if (isDoor(prop->name())) {
    prop->spritesPlaybackControls().play(game::PlayDirection::Forward, game::LoopMode::Stop);
  }
}

void MainMenuSceneController::onPropExit(game::Scene& scene, game::PropId propId) {
  SceneController::onPropExit(scene, propId);

  auto* prop = scene.prop(propId);
  if (isDoor(prop->name())) {
    prop->spritesPlaybackControls().play(game::PlayDirection::Backward, game::LoopMode::Stop);
  }
}

void MainMenuSceneController::onPropClicked(game::Scene& scene, game::PropId propId) {
  SceneController::onPropClicked(scene, propId);

  auto propData = scene.prop(propId);
  if (propData) {
    if (propData->name() == "pilotdr") {
      sceneManager().switchToScene("register");
    }
  }
}
void MainMenuSceneController::onUpdate(game::Scene& scene, U32 millis) {
  SceneController::onUpdate(scene, millis);
}

}  // namespace xwing
