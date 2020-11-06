#pragma once

#include <engine/stage.h>

#include "game/resources.h"
#include "scene_renderer.h"

namespace game {

class GameStage : public engine::Stage {
public:
  explicit GameStage(std::shared_ptr<Resources> resources);

  virtual void onRenderGameScreen() = 0;
  virtual void onRenderDebugInfo();

  // Override: engine::Stage
  bool onLoad() override;
  void onStageResized(I32 width, I32 height) override;
  void onRender() override final;

protected:
  // Override: engine::Stage
  bool attachToEngine(renderer::Renderer* renderer) override;
  void detachFromEngine() override;

  std::shared_ptr<Resources> m_resources;

  SceneRenderer m_sceneRenderer;
  RectI m_gameScreenRect{0, 0, 0, 0};

private:
  renderer::RenderTargetId m_gameScreen;
};

}  // namespace game
