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

  m_scene = loadFilm(m_resources.get(), &m_spriteRenderer, this, m_filmName);

  return !!m_scene;
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

  spdlog::info("Mouse position: ({}, {})", x, y);
}

void SingleSceneStage::onUpdate(U32 millis) {
  if (m_scene) {
    m_scene->update(millis);
  }
}

void SingleSceneStage::onRenderGameScreen() {
  if (m_scene) {
    m_scene->render();
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

// static
std::unique_ptr<Scene> SingleSceneStage::loadFilm(Resources* resources,
                                                  renderer::SpriteRenderer* renderer,
                                                  SceneDelegate* sceneDelegate,
                                                  std::string_view name) {
  auto scene = std::make_unique<Scene>(sceneDelegate, resources, renderer);

  // Apply the default palette.
  if (!scene->loadPalette("standard")) {
    return {};
  }

  spdlog::info("Loading film: {}", name);

  // Load the specified film.
  if (!scene->loadFilm(name)) {
    return {};
  }

  return scene;
}

}  // namespace game
