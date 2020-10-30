#pragma once

#include <renderer/renderer.h>
#include <renderer/sprite_renderer.h>

#include <memory>

#include "engine/resources.h"
#include "engine/scene.h"
#include "engine/stage.h"
#include "true_type_font.h"

namespace engine {

class Engine {
public:
  void addResourceFile(const ResourceFile& resourceFile);
  void setStage(std::unique_ptr<Stage> stage);

  // Lifecycle
  bool init(std::string_view windowTitle);
  void run();

private:
  void update(U32 ticks);
  void renderGameScreen();
  void renderOverlay();

  Resources m_resources;
  renderer::Renderer m_renderer;
  renderer::SpriteRenderer m_spriteRenderer;

  SDL_Window* m_window;
  SDL_GLContext m_context;
  renderer::RenderTargetId m_screen;

  std::unique_ptr<Stage> m_currentStage;
};

}  // namespace engine
