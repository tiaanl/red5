#pragma once

#include <engine/stage.h>

#include "game/resources.h"

namespace game {

class GameStage : public engine::Stage {
public:
  explicit GameStage(std::shared_ptr<Resources> resources);

  virtual void onRenderGameScreen() = 0;

  bool onLoad() override;
  void onRender() override final;

protected:
  // Override: engine::Stage
  void attachToEngine(renderer::Renderer* renderer) override;
  void detachFromEngine() override;

  std::shared_ptr<Resources> m_resources;

  renderer::SpriteRenderer m_spriteRenderer;
  renderer::RenderTargetId m_gameScreen;
};

}  // namespace game
