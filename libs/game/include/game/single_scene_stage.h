#pragma once

#include <engine/stage.h>

#include "game/scene.h"
#include "game/game_stage.h"

namespace game {

class SingleSceneStage : public GameStage, public SceneDelegate {
public:
  SingleSceneStage(std::shared_ptr<Resources> resources, std::string_view filmName);

  bool addResourceFile(const ResourceFile& resourceFile);

  // Override: engine::Stage
  bool onReady() override;
  void onUpdate(U32 millis) override;
  void onRender() override;

  // Override: SceneDelegate
  void onSceneReady() override;
  void onSceneEvent(I16 event) override;
  void onSceneLastFramePlayed() override;

private:
  static std::unique_ptr<Scene> loadFilm(Resources* resources, renderer::SpriteRenderer* renderer,
                                         SceneDelegate* sceneDelegate, std::string_view name);

  std::string m_filmName;

  std::unique_ptr<Scene> m_scene;
};

}  // namespace game
