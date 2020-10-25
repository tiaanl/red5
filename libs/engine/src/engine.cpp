#include "engine/engine.h"

#include <SDL2/SDL.h>
#include <spdlog/sinks/msvc_sink.h>

namespace engine {

namespace {

constexpr U16 g_screenWidth = 320;
constexpr U16 g_screenHeight = 200;

}  // namespace

void Engine::addResourceFile(const ResourceFile& resourceFile) {
  m_resources.addResourceFile(resourceFile);
}

void Engine::setStage(std::unique_ptr<Stage> stage) {
  spdlog::info("Setting stage");

  // m_ttf.loadFromFont(m_sdlRenderer);

  if (m_currentStage) {
    m_currentStage->detachFromEngine();
  }

  m_currentStage = std::move(stage);

  m_currentStage->attachToEngine(&m_resources, &m_renderer);
  m_currentStage->onReady();
}

bool Engine::init(std::string_view windowTitle) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return false;
  }

  spdlog::default_logger()->sinks().push_back(std::make_shared<spdlog::sinks::windebug_sink_st>());

  constexpr I32 screenScale = 5;

  m_window = SDL_CreateWindow(windowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              g_screenWidth * screenScale, g_screenHeight * screenScale,
                              SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);

  m_renderer.init(m_window);

  m_screen = m_renderer.createRenderTarget(g_screenWidth, g_screenHeight);

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
      }
    }

    auto now = SDL_GetTicks();
    update(now - lastTicks);
    lastTicks = now;

    m_renderer.setRenderTarget(m_screen);
    m_renderer.clear(0, 0, 0, 255);

    renderGameScreen();

    m_renderer.clearRenderTarget();

    SDL_Point windowSize;
    SDL_GetWindowSize(m_window, &windowSize.x, &windowSize.y);

    F32 scale = static_cast<F32>(windowSize.x) / static_cast<F32>(g_screenWidth);
    I32 screenHeight = static_cast<I32>(std::round(static_cast<F32>(g_screenHeight) * scale));
    if (screenHeight > windowSize.y) {
      scale = static_cast<F32>(windowSize.y) / static_cast<F32>(g_screenHeight);
      screenHeight = static_cast<I32>(std::round(static_cast<F32>(g_screenHeight) * scale));
    }

    I32 screenWidth = static_cast<I32>(std::round(static_cast<F32>(g_screenWidth) * scale));

    renderer::Rect destination{windowSize.x / 2 - screenWidth / 2,
                               windowSize.y / 2 - screenHeight / 2, screenWidth, screenHeight};

    m_renderer.copyTexture(m_screen, destination);

    // renderOverlay();

    m_renderer.present();
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
