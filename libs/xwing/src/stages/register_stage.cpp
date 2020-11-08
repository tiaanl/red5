#include "xwing/stages/register_stage.h"

namespace xwing {

RegisterStage::RegisterStage(std::shared_ptr<game::Resources> resources)
  : SingleSceneStage{std::move(resources), "pilot"} {}

RegisterStage::~RegisterStage() = default;

void RegisterStage::onUpdate(U32 millis) {
  SingleSceneStage::onUpdate(millis);

  m_robotMillis += millis;
  if (m_robotMillis > 1000) {
    m_robotMillis %= 1000;

    if (std::rand() % 10 < 3) {
      auto* prop = m_scene->prop(m_robotHeadId);
      prop->spritesPlaybackControls().jumpToRandomFrame();
    }
    if (std::rand() % 10 < 3) {
      auto* prop = m_scene->prop(m_robotArmRightId);
      prop->spritesPlaybackControls().jumpToRandomFrame();
    }
    if (std::rand() % 10 < 3) {
      auto* prop = m_scene->prop(m_robotArmLeftId);
      prop->spritesPlaybackControls().jumpToRandomFrame();
    }
  }
}

bool RegisterStage::onLoad(game::Scene& scene) {
  if (!SingleSceneStage::onLoad(scene)) {
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

  if (!initRobot(scene)) {
    return false;
  }

  return true;
}

void RegisterStage::onPropEnter(game::PropId propId) {
  SingleSceneStage::onPropEnter(propId);

  if (propId == m_doorId) {
    spdlog::info("Opening door");
    auto* door = m_scene->prop(m_doorId);
    door->spritesPlaybackControls().play(game::PlayDirection::Forward, game::LoopMode::Stop);
  }
}

void RegisterStage::onPropExit(game::PropId propId) {
  SingleSceneStage::onPropExit(propId);

  if (propId == m_doorId) {
    spdlog::info("Closing door");
    auto* door = m_scene->prop(m_doorId);
    door->spritesPlaybackControls().play(game::PlayDirection::Backward, game::LoopMode::Stop);
  }
}
bool RegisterStage::initRobot(game::Scene& scene) {
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

  m_robotBodyId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(0, 1);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 40;
  });
  if (!m_robotBodyId) {
    return false;
  }

  m_robotHeadId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(1, 5);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 40;
  });
  if (!m_robotHeadId) {
    return false;
  }

  m_robotArmLeftId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(6, 4);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 10;
  });
  if (!m_robotArmLeftId) {
    return false;
  }

  m_robotArmRightId = scene.insertAnimation("robot", [](game::Prop& prop) {
    prop.setAnimate(false);
    prop.spritesPlaybackControls().setFrameRange(10, 4);
    prop.currentFrame().visible = true;
    prop.currentFrame().layer = 10;
  });
  if (!m_robotArmRightId) {
    return false;
  }

  return true;
}

}  // namespace xwing
