#pragma once

#include <renderer/renderer.h>
#include <renderer/sprite_renderer.h>

#include <memory>

#include "engine/stage.h"
#include "true_type_font.h"

namespace engine {

class Engine {
public:
  bool setStage(std::unique_ptr<Stage> stage);

  // Lifecycle
  bool init(std::string_view windowTitle);
  void run();

private:
  void update(U32 ticks);

  renderer::Renderer m_renderer;

  SDL_Window* m_window;
  SDL_GLContext m_context;

  renderer::Size m_windowSize{0, 0};
  std::unique_ptr<Stage> m_currentStage;
};

}  // namespace engine
