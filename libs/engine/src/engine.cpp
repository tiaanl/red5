#include "engine/engine.h"

#include <SDL2/SDL.h>
#include <spdlog/sinks/msvc_sink.h>

namespace engine {

namespace {

constexpr I32 g_screenScale = 5;

constexpr U16 g_screenWidth = 320;
constexpr U16 g_screenHeight = 200;

}  // namespace

void Engine::addResourceFile(const ResourceFile& resourceFile) {
  m_resources.addResourceFile(resourceFile);
}

void Engine::setStage(std::unique_ptr<Stage> stage) {
  spdlog::info("Setting stage");

  if (m_currentStage) {
    m_currentStage->detachFromEngine();
  }

  m_currentStage = std::move(stage);

  m_currentStage->attachToEngine(&m_resources, &m_spriteRenderer);
  m_currentStage->onReady();
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
      windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      g_screenWidth * g_screenScale, g_screenHeight * g_screenScale,
      SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (!m_window) {
    SDL_Quit();
    spdlog::critical("Could not create window. ({})", SDL_GetError());
    return false;
  }

  m_context = SDL_GL_CreateContext(m_window);
  if (!m_context) {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    spdlog::critical("Could not create OpenGL context. ({})", SDL_GetError());
    return false;
  }

  if (!m_renderer.init(m_window)) {
    return false;
  }

  m_screen = m_renderer.renderTargets().create({g_screenWidth, g_screenHeight});

  if (!m_spriteRenderer.attachToRenderer(&m_renderer)) {
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
      } else if (event.type == SDL_WINDOWEVENT_RESIZED) {
        m_renderer.resize({event.window.data1, event.window.data2});
      }
    }

    auto now = SDL_GetTicks();
    update(now - lastTicks);
    lastTicks = now;

    m_renderer.beginFrame();

    m_renderer.setRenderTarget(m_screen);
    m_renderer.clear(0.0f, 0, 0, 1.0f);
    renderGameScreen();
    m_renderer.clearRenderTarget();

    m_renderer.clear(0.0f, 0.5f, 0.0f, 1.0f);

    SDL_Point windowSize;
    SDL_GetWindowSize(m_window, &windowSize.x, &windowSize.y);

    renderer::Rect destination{renderer::fitInto({0, 0, g_screenWidth, g_screenHeight},
                                                 {0, 0, windowSize.x, windowSize.y})};

    m_renderer.renderRenderTarget(m_screen, destination);

    renderOverlay();

    m_renderer.finishFrame();
  }

  SDL_Quit();
}

void Engine::update(U32 ticks) {
  if (m_currentStage) {
    m_currentStage->onUpdate(ticks);
  }
}

void Engine::renderGameScreen() {
  if (m_currentStage) {
    m_currentStage->onRender();
  }
}

void Engine::renderOverlay() {
  // m_ttf.renderText(&m_renderer, {10, 10}, "test");
}

}  // namespace engine
