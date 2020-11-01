#include "game/single_scene_stage.h"

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
  m_mouseCursor = m_scene->insertAnimation("cursors", {});
  if (!m_mouseCursor) {
    return false;
  }

  auto prop = m_scene->props().getData(m_mouseCursor);
  prop->setSpriteIndex(0);
  prop->setLayer(0);

  return true;
}

void SingleSceneStage::onMouseMoved(I32 x, I32 y) {
  Stage::onMouseMoved(x, y);

#if 0
  spdlog::info("rect: (({}, {}), ({}, {}))", m_gameScreenRect.position.left,
               m_gameScreenRect.position.top, m_gameScreenRect.size.width,
               m_gameScreenRect.size.height);
#endif  // 0

  F32 mouseX = static_cast<F32>(x) - static_cast<F32>(m_gameScreenRect.position.left);
  mouseX = std::min(static_cast<F32>(m_gameScreenRect.size.width), mouseX);
  mouseX = std::max(0.0f, mouseX);
  mouseX = mouseX / static_cast<F32>(m_gameScreenRect.size.width) * static_cast<F32>(320);

  F32 mouseY = static_cast<F32>(y) - static_cast<F32>(m_gameScreenRect.position.top);
  mouseY = std::min(static_cast<F32>(m_gameScreenRect.size.height), mouseY);
  mouseY = std::max(0.0f, mouseY);
  mouseY = mouseY / static_cast<F32>(m_gameScreenRect.size.height) * static_cast<F32>(200);

  x = static_cast<I32>(std::round(mouseX));
  y = static_cast<I32>(std::round(mouseY));

  // spdlog::info("Mouse position: ({}, {})", x, y);

  auto prop = m_scene->props().getData(m_mouseCursor);
  prop->setOffset({x, y});
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
}

void SingleSceneStage::onRenderDebugInfo() {
  if (m_scene) {
    m_scene->renderDebugInfo();
  }
}

void SingleSceneStage::onSceneReady() {
  SceneDelegate::onSceneReady();
}

void SingleSceneStage::onSceneEvent(I16 event) {
  SceneDelegate::onSceneEvent(event);
}
void SingleSceneStage::onSceneLastFramePlayed() {
  SceneDelegate::onSceneLastFramePlayed();
}

}  // namespace game
