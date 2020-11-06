#include "engine/engine.h"

#include <SDL2/SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <renderer/render_shapes.h>
#include <spdlog/sinks/msvc_sink.h>

namespace engine {

bool Engine::setStage(std::unique_ptr<Stage> stage) {
  // spdlog::info("Setting stage");

  if (m_currentStage) {
    m_currentStage->detachFromEngine();
  }

  m_currentStage = std::move(stage);

  if (!m_currentStage->attachToEngine(&m_renderer)) {
    return false;
  }

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

#ifdef __APPLE__
  // GL 3.2 Core + GLSL 150
  const char* glslVersion = "#version 150";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
  // GL 3.0 + GLSL 130
  const char* glslVersion = "#version 130";
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

  m_window = SDL_CreateWindow(windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1600, 1000, windowFlags);

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

  SDL_GL_MakeCurrent(m_window, m_context);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  SDL_GetWindowSize(m_window, &m_windowSize.width, &m_windowSize.height);

  if (!m_renderer.init(m_window)) {
    return false;
  }

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

  return true;
}

void Engine::run() {
  SDL_ShowWindow(m_window);

  U32 lastTicks = SDL_GetTicks();
  bool running = true;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);

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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();

    if (m_currentStage) {
      m_currentStage->onRender();
    }

    m_renderer.flushRenderQueue();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_renderer.finishFrame();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(m_context);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void Engine::update(U32 ticks) {
  if (m_currentStage) {
    m_currentStage->onUpdate(ticks);
  }
}

}  // namespace engine
