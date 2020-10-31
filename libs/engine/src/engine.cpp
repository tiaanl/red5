#include "engine/engine.h"

#include <SDL2/SDL.h>
#include <spdlog/sinks/msvc_sink.h>

namespace engine {

bool Engine::setStage(std::unique_ptr<Stage> stage) {
  // spdlog::info("Setting stage");

  if (m_currentStage) {
    m_currentStage->detachFromEngine();
  }

  m_currentStage = std::move(stage);

  m_currentStage->attachToEngine(&m_renderer);

  if (!m_currentStage->onLoad()) {
    return false;
  }

  // Trigger an onStageResized so that the stage can configure any size dependent objects.
  m_currentStage->onStageResized(m_windowSize.width, m_windowSize.height);

  return true;
}

bool Engine::init(std::string_view windowTitle) {
  spdlog::default_logger()->sinks().push_back(std::make_shared<spdlog::sinks::windebug_sink_st>());

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    spdlog::error("Could not initialize SDL video.");
    return false;
  }

  // Set up OpenGL properties for SDL.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  m_window = SDL_CreateWindow(
      windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 1000,
      SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (!m_window) {
    SDL_Quit();
    spdlog::critical("Could not create window. ({})", SDL_GetError());
    return false;
  }

  SDL_ShowCursor(SDL_DISABLE);

  m_context = SDL_GL_CreateContext(m_window);
  if (!m_context) {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    spdlog::critical("Could not create OpenGL context. ({})", SDL_GetError());
    return false;
  }

  SDL_GetWindowSize(m_window, &m_windowSize.width, &m_windowSize.height);

  if (!m_renderer.init(m_window)) {
    return false;
  }

  return true;
}

void Engine::run() {
  SDL_ShowWindow(m_window);

  U32 lastTicks = SDL_GetTicks();
  bool running = true;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        break;
      } else if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
          m_windowSize = {event.window.data1, event.window.data2};
          m_renderer.resize(m_windowSize);
          if (m_currentStage) {
            m_currentStage->onStageResized(m_windowSize.width, m_windowSize.height);
          }
        }
      } else if (event.type == SDL_MOUSEMOTION) {
        if (m_currentStage) {
          m_currentStage->onMouseMoved(event.motion.x, event.motion.y);
        }
      }
    }

    auto now = SDL_GetTicks();
    update(now - lastTicks);
    lastTicks = now;

    m_renderer.beginFrame();

    if (m_currentStage) {
      m_currentStage->onRender();
    }

    m_renderer.finishFrame();
  }

  SDL_Quit();
}

void Engine::update(U32 ticks) {
  if (m_currentStage) {
    m_currentStage->onUpdate(ticks);
  }
}

}  // namespace engine
