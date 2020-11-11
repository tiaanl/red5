#include "engine/engine.h"

#include <SDL2/SDL.h>

#if defined(_WIN32)
#include <spdlog/sinks/msvc_sink.h>
#endif

#if DEBUG_UI > 0
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#endif  // DEBUG_UI > 0

namespace engine {

Engine::Engine() = default;

Engine::~Engine() {
  m_renderer.reset();

  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void Engine::setStage(std::unique_ptr<Stage> stage) {
  m_engineOps.switchStage(std::move(stage));
}

bool Engine::run() {
  SDL_ShowWindow(m_window);

  mainLoop();

#if DEBUG_UI > 0
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
#endif  // DEBUG_UI > 0

  // Reset the current stage to allow it to free up resources.
  m_currentStage.reset();

  return true;
}

bool Engine::init(std::string_view title) {
#if defined(_WIN32)
  spdlog::default_logger()->sinks().push_back(std::make_shared<spdlog::sinks::windebug_sink_st>());
#endif

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    spdlog::error("Could not initialize SDL video.");
    return false;
  }

#ifdef __APPLE__
#if DEBUG_UI > 0
  // GL 3.2 Core + GLSL 150
  const char* glslVersion = "#version 150";
#endif  // DEBUG_UI > 0
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
#if DEBUG_UI > 0
  // GL 3.0 + GLSL 130
  const char* glslVersion = "#version 130";
#endif  // DEBUG_UI > 0
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL |
                                                  SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  m_window = SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600,
                              1000, windowFlags);

  if (!m_window) {
    SDL_Quit();
    spdlog::critical("Could not init window. ({})", SDL_GetError());
    return false;
  }

  // SDL_ShowCursor(SDL_DISABLE);

  m_context = SDL_GL_CreateContext(m_window);
  if (!m_context) {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    spdlog::critical("Could not init OpenGL context. ({})", SDL_GetError());
    return false;
  }

  SDL_GL_MakeCurrent(m_window, m_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  SDL_GetWindowSize(m_window, &m_windowSize.width, &m_windowSize.height);

  m_renderer.emplace();
  if (!m_renderer->init(m_window)) {
    return false;
  }

#if DEBUG_UI > 0
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
  ImGui_ImplOpenGL3_Init(glslVersion);
#endif  // DEBUG_UI > 0

  return true;
}

bool Engine::swapStage(std::unique_ptr<Stage> newStage) {
  if (m_currentStage) {
    m_currentStage->detachFromEngine();
  }

  m_currentStage = std::move(newStage);

  if (!m_currentStage->attachToEngine(&m_engineOps, &m_renderer.value())) {
    spdlog::error("Could not attach stage to engine.");
    return false;
  }

  // Trigger an onStageResized so that the stage can configure any size dependent objects.
  m_currentStage->onStageResized(m_windowSize.width, m_windowSize.height);

  return true;
}

void Engine::mainLoop() {
  U32 lastTicks = SDL_GetTicks();

  for (;;) {
    // If the current stage requested a new stage, then we swap in the new stage first.
    if (m_engineOps.m_switchToStage) {
      if (!swapStage(std::move(m_engineOps.m_switchToStage))) {
        spdlog::error("Could not swap stage.");
        break;
      }
    }

    if (!processEvents()) {
      break;
    }

    auto now = SDL_GetTicks();
    update(now - lastTicks);
    lastTicks = now;

#if DEBUG_UI > 0
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();
#endif  // DEBUG_UI > 0

    if (m_currentStage) {
      m_currentStage->onRender();
    }

    m_renderer->flushRenderQueue();

#if DEBUG_UI > 0
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif  // DEBUG_UI > 0

    m_renderer->finishFrame();
  }
}

bool Engine::processEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
#if DEBUG_UI > 0
    ImGui_ImplSDL2_ProcessEvent(&event);
#endif  // DEBUG_UI > 0

    switch (event.type) {
      case SDL_QUIT: {
        return false;
      }

      case SDL_WINDOWEVENT: {
        switch (event.window.event) {
          case SDL_WINDOWEVENT_RESIZED: {
            m_windowSize = {event.window.data1, event.window.data2};
            m_renderer->resize(m_windowSize);
            if (m_currentStage) {
              m_currentStage->onStageResized(m_windowSize.width, m_windowSize.height);
            }
            break;
          }
        }
        break;
      }

      case SDL_MOUSEMOTION: {
        if (m_currentStage) {
          m_currentStage->onMouseMoved({event.motion.x, event.motion.y});
        }
        break;
      }

      case SDL_MOUSEBUTTONDOWN: {
        if (m_currentStage) {
          m_currentStage->onMousePressed({event.button.x, event.button.y}, event.button.button);
        }
        break;
      }

      case SDL_MOUSEBUTTONUP: {
        if (m_currentStage) {
          m_currentStage->onMouseReleased({event.button.x, event.button.y}, event.button.button);
        }
        break;
      }
    }
  }

  return true;
}

void Engine::update(U32 ticks) {
  if (m_currentStage) {
    m_currentStage->onUpdate(ticks);
  }
}

}  // namespace engine
