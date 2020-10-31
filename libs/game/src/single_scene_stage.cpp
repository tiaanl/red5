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
