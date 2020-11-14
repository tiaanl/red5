#include "game/scene_stage.h"

namespace game {

SceneStage::SceneStage(std::shared_ptr<GameStageState> gameStageState,
                       std::unique_ptr<SceneController> controller)
  : m_gameStageState{std::move(gameStageState)}, m_controller{std::move(controller)} {}

SceneStage::~SceneStage() = default;

bool SceneStage::onAttachedToEngine(engine::Renderer* renderer) {
  Resources& resources = m_gameStageState->resources;
  m_scene = std::make_unique<Scene>(nullptr, &resources, &m_gameStageState->sceneRenderer);

  if (!m_controller->setUpScene(*m_scene)) {
    return false;
  }

  return true;
}

void SceneStage::onDetachFromEngine() {}

void SceneStage::onStageResized(I32 width, I32 height) {
  Stage::onStageResized(width, height);

  m_gameScreenRect.size = {width, height};
}

void SceneStage::onMouseMoved(const PositionI& mousePosition) {
  Stage::onMouseMoved(mousePosition);

  auto gameScreenPosition = windowCoordToSceneCoord(mousePosition);

  if (m_scene) {
    setPropUnderMouse(m_scene->propUnderMouse(gameScreenPosition));
  }
}

void SceneStage::onMousePressed(const PositionI& mousePosition, U8 buttons) {
  Stage::onMousePressed(mousePosition, buttons);

  m_propUnderMousePress = m_propUnderMouse;
}

void SceneStage::onMouseReleased(const PositionI& mousePosition, U8 buttons) {
  Stage::onMouseReleased(mousePosition, buttons);

  auto gameScreenPosition = windowCoordToSceneCoord(mousePosition);
  auto propUnderMouse = m_scene->propUnderMouse(gameScreenPosition);

  if (propUnderMouse == m_propUnderMousePress && m_controller) {
    m_controller->onPropClicked(*m_scene, propUnderMouse);
  }

  m_propUnderMousePress = PropId::invalidValue();
}

void SceneStage::onUpdate(U32 millis) {
  if (m_controller) {
    m_controller->onUpdate(*m_scene, millis);
  }
  if (m_scene) {
    m_scene->update(millis);
  }
}

void SceneStage::onRender() {
  auto& r = renderer();

  if (m_scene) {
    r.setRenderTarget(m_gameStageState->gameScreenRenderTarget);
    r.clear(0.0f, 0, 0, 1.0f);
    m_scene->renderGameScreen();
    r.clearRenderTarget();
  }

  r.clear(0.0f, 0.5f, 0.0f, 1.0f);
  r.copyRenderTarget(m_gameScreenRect, m_gameStageState->gameScreenRenderTarget, {0, 0, 320, 200});
}
PositionI SceneStage::windowCoordToSceneCoord(const PositionI& windowCoord) {
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
void SceneStage::setPropUnderMouse(PropId propId) {
  if (m_propUnderMouse == propId) {
    return;
  }

  if (m_propUnderMouse.isValid() && m_controller) {
    m_controller->onPropExit(*m_scene, m_propUnderMouse);
  }

  m_propUnderMouse = propId;

  if (propId.isValid() && m_controller) {
    m_controller->onPropEnter(*m_scene, propId);
  }
}

}  // namespace game
