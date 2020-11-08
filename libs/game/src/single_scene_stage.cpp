#include "game/single_scene_stage.h"

#include <engine/utils/render_shapes.h>

namespace game {

SingleSceneStage::SingleSceneStage(std::shared_ptr<Resources> resources, std::string_view filmName)
  : GameStage{std::move(resources)}, m_filmName{filmName} {}

bool SingleSceneStage::addResourceFile(const ResourceFile& resourceFile) {
  return m_resources->addResourceFile(resourceFile);
}

bool SingleSceneStage::onLoad() {
  if (!GameStage::onLoad()) {
    return false;
  }

  m_scene = std::make_unique<Scene>(this, m_resources.get(), &m_sceneRenderer);

  // Apply the default palette.
  if (!m_scene->loadPalette("standard")) {
    return false;
  }

  spdlog::info("Loading film: {}", m_filmName);

  if (!m_scene->loadFilm(m_filmName)) {
    return false;
  }

  // Insert the mouse cursor to the scene.
  m_mouseCursor = m_scene->insertAnimation("cursors");
  if (!m_mouseCursor) {
    return false;
  }

  auto* prop = m_scene->props().getData(m_mouseCursor);
  auto& frame = prop->currentFrame();
  frame.visible = true;
  frame.spriteIndex = 0;
  frame.layer = 0;

  if (!onLoad(*m_scene)) {
    return false;
  }

  return true;
}

void SingleSceneStage::onMouseMoved(const PositionI& mousePosition) {
  Stage::onMouseMoved(mousePosition);

  auto gameScreenPosition = windowCoordToSceneCoord(mousePosition);

  auto mouseProp = m_scene->props().getData(m_mouseCursor);
  mouseProp->moveTo(gameScreenPosition);

  if (m_scene) {
    setPropUnderMouse(m_scene->propUnderMouse(gameScreenPosition));
  }
}

void SingleSceneStage::onUpdate(U32 millis) {
  if (m_scene) {
    m_scene->update(millis);
  }
}

void SingleSceneStage::onRenderGameScreen() {
  if (m_scene) {
    m_scene->renderGameScreen();
  }

#if 0
  if (m_propUnderMouse.isValid()) {
    auto prop = m_scene->prop(m_propUnderMouse);
    engine::renderRectangle(*m_renderer, prop->bounds(), {1.0f, 0.0f, 0.0f, 0.5f});
  }
#endif  // 0
}

#if DEBUG_UI > 0
void SingleSceneStage::onRenderDebugInfo() {
  if (m_scene) {
    m_scene->renderDebugInfo();
  }
}
#endif  // DEBUG_UI > 0

void SingleSceneStage::onSceneReady() {
  SceneDelegate::onSceneReady();
}

void SingleSceneStage::onSceneEvent(I16 event) {
  SceneDelegate::onSceneEvent(event);
}

void SingleSceneStage::onSceneLastFramePlayed() {
  SceneDelegate::onSceneLastFramePlayed();
}

bool SingleSceneStage::onLoad(Scene& scene) {
  return true;
}

void SingleSceneStage::onPropEnter(PropId propId) {
#if 0
  auto prop = m_scene->prop(propId);
  spdlog::info("enter: {}", prop->name());
#endif  // 0
}

void SingleSceneStage::onPropExit(PropId propId) {
#if 0
  auto prop = m_scene->prop(propId);
  spdlog::info("exit: {}", prop->name());
#endif  // 0
}

PositionI SingleSceneStage::windowCoordToSceneCoord(const PositionI& windowCoord) {
  RectF gameScreenRect = m_gameScreenRect;
  PositionF p = windowCoord;

  F32 mouseX = p.left - gameScreenRect.position.left;
  mouseX = std::min(gameScreenRect.size.width, mouseX);
  mouseX = std::max(0.0f, mouseX);
  mouseX = mouseX / gameScreenRect.size.width * 320.0f;

  F32 mouseY = windowCoord.top - gameScreenRect.position.top;
  mouseY = std::min(gameScreenRect.size.height, mouseY);
  mouseY = std::max(0.0f, mouseY);
  mouseY = mouseY / gameScreenRect.size.height * 200.0f;

  return PositionI{mouseX, mouseY};
}

void SingleSceneStage::setPropUnderMouse(PropId propId) {
  if (m_propUnderMouse == propId) {
    return;
  }

  if (m_propUnderMouse.isValid()) {
    onPropExit(m_propUnderMouse);
  }

  m_propUnderMouse = propId;

  if (propId.isValid()) {
    onPropEnter(propId);
  }
}

}  // namespace game
