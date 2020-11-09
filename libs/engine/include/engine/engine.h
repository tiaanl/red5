#pragma once

#include <SDL_events.h>

#include <memory>

#include "engine/engine_ops.h"
#include "engine/renderer/renderer.h"

namespace engine {

class Stage;

class Engine {
public:
  bool setStage(std::unique_ptr<Stage> stage);

  // Lifecycle
  bool init(std::string_view windowTitle);
  void run();

private:
  bool processEvents();
  void update(U32 ticks);

  void mainLoop();

  Renderer m_renderer;

  SDL_Window* m_window;
  SDL_GLContext m_context;

  SizeI m_windowSize{0, 0};
  std::unique_ptr<Stage> m_currentStage;

  EngineOps m_engineOps;
};

}  // namespace engine
