#include "engine/single_scene_stage.h"

namespace engine {

SingleSceneStage::SingleSceneStage(std::string_view filmName) : m_filmName{filmName} {}

bool SingleSceneStage::onReady() {
  m_scene = loadFilm(resources(), renderer(), this, m_filmName);

  return !!m_scene;
}

void SingleSceneStage::onUpdate(U32 millis) {
  if (m_scene) {
    m_scene->update(millis);
  }
}

void SingleSceneStage::onRender() {
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
std::unique_ptr<Scene> SingleSceneStage::loadFilm(engine::Resources* resources,
                                                  renderer::Renderer* renderer,
                                                  engine::SceneDelegate* sceneDelegate,
                                                  std::string_view name) {
  auto scene = std::make_unique<engine::Scene>(sceneDelegate, resources, renderer);

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

}  // namespace engine
