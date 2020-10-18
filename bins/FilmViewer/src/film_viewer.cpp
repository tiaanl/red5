
#include <SDL.h>
#include <lfd/image.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>

#include "scene.h"

constexpr U16 g_screenScale = 5;
constexpr U16 g_screenWidth = 320;
constexpr U16 g_screenHeight = 200;

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return 1;
  }

  SDL_Window* window =
      SDL_CreateWindow("X-Wing Film Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       g_screenWidth * g_screenScale, g_screenHeight * g_screenScale,
                       SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_Texture* screen = SDL_CreateTexture(
      renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, g_screenWidth, g_screenHeight);

  Scene scene;

  // Load the standard palette from the XWING base LFD file.
  scene.addResources(ResourceFile{R"(C:\xwing\RESOURCE\XWING.LFD)"});
  scene.loadPalette("standard");

  scene.addResources(ResourceFile{R"(C:\xwing\RESOURCE\DFIRE.LFD)"});
  if (!scene.loadFilm("dfire2_f")) {
    return 1;
  }

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

#if 0
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
#endif  // 0

    auto now = SDL_GetTicks();
    scene.update(now - lastTicks);
    lastTicks = now;

    SDL_Color* pixels;
    I32 pitch;
    if (SDL_LockTexture(screen, nullptr, (void**)&pixels, &pitch) == 0) {
      std::memset(pixels, 0, pitch * g_screenHeight);

      scene.render(pixels);

      SDL_UnlockTexture(screen);
      SDL_RenderCopy(renderer, screen, nullptr, nullptr);
    }

    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}
