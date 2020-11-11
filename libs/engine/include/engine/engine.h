#pragma once

#include <SDL_events.h>

#include <memory>
#include <optional>

#include "engine/engine_ops.h"
#include "engine/renderer/renderer.h"
#include "engine/stage.h"

namespace engine {

class Stage;

class Engine {
public:
  Engine();
  ~Engine();

  void setStage(std::unique_ptr<Stage> stage);

  Renderer& renderer() {
    assert(m_renderer.has_value());
    return m_renderer.value();
  }

  // Lifecycle
  bool run();
  bool init(std::string_view title);

private:
  bool swapStage(std::unique_ptr<Stage> newStage);

  void mainLoop();
  bool processEvents();
  void update(U32 ticks);

  std::optional<Renderer> m_renderer;

  SDL_Window* m_window = nullptr;
  SDL_GLContext m_context = nullptr;

  SizeI m_windowSize;
  std::unique_ptr<Stage> m_currentStage;

  EngineOps m_engineOps;
};

}  // namespace engine
