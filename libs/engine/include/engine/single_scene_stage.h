#pragma once

#include "engine/scene.h"
#include "engine/stage.h"

namespace engine {

class SingleSceneStage : public engine::Stage, public engine::SceneDelegate {
public:
  explicit SingleSceneStage(std::string_view filmName);

  // Override: engine::Stage
  bool onReady() override;
  void onUpdate(U32 millis) override;
  void onRender() override;

  // Override: engine::SceneDelegate
  void onSceneReady() override;
  void onSceneEvent(I16 event) override;
  void onSceneLastFramePlayed() override;

private:
  static std::unique_ptr<Scene> loadFilm(engine::Resources* resources,
                                         renderer::SpriteRenderer* renderer,
                                         engine::SceneDelegate* sceneDelegate,
                                         std::string_view name);

  std::string m_filmName;
  std::unique_ptr<Scene> m_scene;
};

}  // namespace engine
