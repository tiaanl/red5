#pragma once

#include <renderer/renderer.h>
#include <renderer/sprite_renderer.h>

#include <memory>

#include "engine/stage.h"
#include "true_type_font.h"

namespace engine {

class Engine {
public:
  void setStage(std::unique_ptr<Stage> stage);

  // Lifecycle
  bool init(std::string_view windowTitle);
  void run();

private:
  void update(U32 ticks);
  void renderGameScreen();
  void renderOverlay();

  renderer::Renderer m_renderer;
  renderer::SpriteRenderer m_spriteRenderer;

  SDL_Window* m_window;
  SDL_GLContext m_context;
  renderer::RenderTargetId m_screen;

  std::unique_ptr<Stage> m_currentStage;
};

}  // namespace engine
