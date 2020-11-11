#include "xwing/stages/main_menu_stage.h"

#include "xwing/stages/register_stage.h"

namespace xwing {

MainMenuStage::MainMenuStage(std::shared_ptr<game::GameStageState> gameStageState)
  : game::SingleSceneStage(std::move(gameStageState), "mainmenu") {}

bool MainMenuStage::onLoad(game::Scene& scene) {
  if (!SingleSceneStage::onLoad(scene)) {
    return false;
  }

  scene.prop(scene.propId("middoor"))->setAnimate(false);
  scene.prop(scene.propId("lhdoor"))->setAnimate(false);
  scene.prop(scene.propId("toddoor"))->setAnimate(false);
  scene.prop(scene.propId("lwr-lhdr"))->setAnimate(false);
  scene.prop(scene.propId("lwr-rhdr"))->setAnimate(false);
  scene.prop(scene.propId("pilotdr"))->setAnimate(false);

  return true;
}

void MainMenuStage::onPropEnter(game::PropId propId) {
  SingleSceneStage::onPropEnter(propId);

  auto* prop = m_scene->prop(propId);
  spdlog::info("enter: {}", prop->name());
  if (isDoor(prop->name())) {
    prop->spritesPlaybackControls().play(game::PlayDirection::Forward, game::LoopMode::Stop);
  }
}

void MainMenuStage::onPropExit(game::PropId propId) {
  SingleSceneStage::onPropExit(propId);

  auto* prop = m_scene->prop(propId);
  if (isDoor(prop->name())) {
    prop->spritesPlaybackControls().play(game::PlayDirection::Backward, game::LoopMode::Stop);
  }
}

void MainMenuStage::onPropClicked(game::PropId propId) {
  SingleSceneStage::onPropClicked(propId);

  auto propData = m_scene->prop(propId);
  if (propData) {
    if (propData->name() == "pilotdr") {
      m_engineOps->switchStage(game::createGameStage<RegisterStage>(m_gameStageState));
    }
  }
}

bool MainMenuStage::isDoor(std::string_view name) const {
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

}  // namespace xwing
