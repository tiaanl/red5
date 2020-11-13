#include "xwing/controllers/register_scene_controller.h"

#include <game/scene_manager.h>

namespace xwing {

bool RegisterSceneController::setUpScene(game::Scene& scene, game::Resources& resources) {
  resources.addResourceFile({R"(C:\XWING\RESOURCE\XWING.LFD)"});
  resources.addResourceFile({R"(C:\XWING\RESOURCE\REGISTER.LFD)"});

  if (!scene.loadPalette("standard")) {
    return false;
  }

  if (!scene.loadFilm("pilot")) {
    return false;
  }

  m_doorId = scene.propId("door");
  if (!m_doorId) {
    return false;
  }
  auto* door = scene.prop(m_doorId);
  door->setAnimate(false);

  scene.insertImage("nameback", [](game::Prop& prop) {
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 1;
  });

  scene.insertImage("infoback", [](game::Prop& prop) {
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 1;
  });

  if (!m_robot.init(scene)) {
    return false;
  }

  return true;
}

void RegisterSceneController::onPropEnter(game::Scene& scene, game::PropId propId) {
  SceneController::onPropEnter(scene, propId);

  if (propId == m_doorId) {
    auto* door = scene.prop(m_doorId);
    door->spritesPlaybackControls().play(game::PlayDirection::Forward, game::LoopMode::Stop);
  }
}

void RegisterSceneController::onPropExit(game::Scene& scene, game::PropId propId) {
  SceneController::onPropExit(scene, propId);

  if (propId == m_doorId) {
    auto* door = scene.prop(m_doorId);
    door->spritesPlaybackControls().play(game::PlayDirection::Backward, game::LoopMode::Stop);
  }
}

void RegisterSceneController::onPropClicked(game::Scene& scene, game::PropId propId) {
  SceneController::onPropClicked(scene, propId);

  if (propId == m_doorId) {
    sceneManager().switchToScene("mainmenu");
  }
}

RegisterSceneController::RegisterSceneController(game::SceneManager* sceneManager)
  : SceneController{sceneManager} {}

void RegisterSceneController::onUpdate(game::Scene& scene, U32 millis) {
  SceneController::onUpdate(scene, millis);

  m_robot.update(scene, millis);
}

bool RegisterSceneController::Robot::init(game::Scene& scene) {
  //  0 = body
  //  1 = head front
  //  2 = head left
  //  3 = head down
  //  4 = head up
  //  5 = head right
  //  6 = right arm
  //  7 = right arm
  //  8 = right arm
  //  9 = right arm
  // 10 = left arm
  // 11 = left arm
  // 12 = left arm
  // 13 = left arm

  bodyId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(0, 1);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 40;
  });
  if (!bodyId) {
    return false;
  }

  headId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(1, 5);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 40;
  });
  if (!headId) {
    return false;
  }

  armLeftId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(6, 4);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 10;
  });
  if (!armLeftId) {
    return false;
  }

  armRightId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(10, 4);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 10;
  });
  if (!armRightId) {
    return false;
  }

  return true;
}

void RegisterSceneController::Robot::update(game::Scene& scene, U32 millis) {
  millisSinceLastUpdate += millis;
  if (millisSinceLastUpdate > 1000) {
    millisSinceLastUpdate %= 1000;

    if (std::rand() % 10 < 4) {
      auto* prop = scene.prop(headId);
      prop->spritesPlaybackControls().jumpToRandomFrame();
    }
    if (std::rand() % 10 < 4) {
      auto* prop = scene.prop(armRightId);
      prop->spritesPlaybackControls().jumpToRandomFrame();
    }
    if (std::rand() % 10 < 4) {
      auto* prop = scene.prop(armLeftId);
      prop->spritesPlaybackControls().jumpToRandomFrame();
    }
  }
}

}  // namespace xwing
