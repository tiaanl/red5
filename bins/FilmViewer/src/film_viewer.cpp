
#include <SDL.h>
#include <engine/scene.h>
#include <spdlog/sinks/msvc_sink.h>

constexpr U16 g_screenScale = 4;
constexpr U16 g_screenWidth = 320;
constexpr U16 g_screenHeight = 200;

class FilmViewerSceneDelegate : public engine::SceneDelegate {};

int main(int argc, char* argv[]) {
  spdlog::default_logger()->sinks().push_back(std::make_shared<spdlog::sinks::windebug_sink_st>());

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return 1;
  }

  SDL_Window* window =
      SDL_CreateWindow("X-Wing Film Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       g_screenWidth * g_screenScale, g_screenHeight * g_screenScale,
                       SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  engine::Resources resources;
  resources.addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\XWING.LFD)"});
  resources.addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\UNIFORM.LFD)"});

  FilmViewerSceneDelegate delegate;
  engine::Scene scene{&delegate, &resources, renderer};

  // Load the standard palette from the XWING base LFD file.
  scene.loadPalette("standard");
  // scene.loadFont("font8");
  scene.loadFont("font6");

  if (!scene.loadFilm("uniform")) {
    return 1;
  }

  SDL_Texture* screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                          SDL_TEXTUREACCESS_TARGET, g_screenWidth, g_screenHeight);

  SDL_ShowWindow(window);

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
    scene.update(now - lastTicks);
    lastTicks = now;

    SDL_SetRenderTarget(renderer, screen);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    scene.render();

    SDL_SetRenderTarget(renderer, nullptr);

    SDL_RenderCopy(renderer, screen, nullptr, nullptr);

    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}
