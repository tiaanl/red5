
#include <SDL.h>
#include <lfd/image.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>
#include <nucleus/Streams/ArrayInputStream.h>

constexpr U16 g_screenScale = 4;
constexpr U16 g_screenWidth = 320;
constexpr U16 g_screenHeight = 200;

struct Color {
  U8 red;
  U8 green;
  U8 blue;
  U8 alpha;
};

using ImagePalette = Color[256];

void drawImage(const Image& image, const ImagePalette& palette, Color* pixels) {
  // LOG(Info) << "Drawing image at (" << image.left() << ", " << image.top() << ")";
  for (auto& line : image.lines()) {
    MemSize pos = line.top * g_screenWidth + line.left;
    for (auto index : line.indices) {
      pixels[pos++] = palette[index];
    }
  }
}

std::unique_ptr<Image> loadImage(const nu::DynamicArray<ResourceEntry>& entries,
                                 nu::StringView name) {
  auto result = std::make_unique<Image>();

  auto imageResource =
      std::find_if(std::begin(entries), std::end(entries), [&name](const ResourceEntry& entry) {
        return entry.type() == ResourceType::Image && entry.name() == name;
      });
  if (imageResource != std::end(entries)) {
    nu::ArrayInputStream stream{
        nu::ArrayView{imageResource->data().data(), imageResource->data().size()}};
    result->read(&stream);
  }

  return result;
}

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

  ResourceFile awards{nu::FilePath{R"(C:\xwing\RESOURCE\MAINMENU.LFD)"}};
  auto entries = awards.loadEntries();

  auto mainmenu = loadImage(entries, "mainmenu");
  auto maindoor = loadImage(entries, "maindoor");
  auto maintitl = loadImage(entries, "maintitl");

  ImagePalette palette = {};

  for (auto& entry : entries) {
    if (entry.type() == ResourceType::Palette && entry.name() == "mainback") {
      nu::ArrayInputStream stream{nu::ArrayView{entry.data().data(), entry.data().size()}};
      Palette p;
      p.read(&stream);

      U8 i = p.firstIndex();
      for (auto& c : p.colors()) {
        palette[i++] = Color{c.red, c.green, c.blue, 255};
      }
    }
  }

  SDL_ShowWindow(window);

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

#if 1
    Color* pixels;
    I32 pitch;
    if (SDL_LockTexture(screen, nullptr, (void**)&pixels, &pitch) == 0) {
      drawImage(*mainmenu, palette, pixels);
      drawImage(*maindoor, palette, pixels);
      drawImage(*maintitl, palette, pixels);

      SDL_UnlockTexture(screen);
      SDL_RenderCopy(renderer, screen, nullptr, nullptr);
    }
#endif  // 0

#if 0
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect{
        img.left() * g_screenScale,
        img.top() * g_screenScale,
        img.right() * g_screenScale,
        img.bottom() * g_screenScale
    };
    SDL_RenderDrawRect(renderer, &rect);
#endif  // 0

    SDL_RenderPresent(renderer);
  }

  SDL_Quit();

  return 0;
}
