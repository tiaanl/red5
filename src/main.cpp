#include <SDL.h>

#include "resource/lfd.h"
#include "resource/film.h"

constexpr uint32_t g_source_screen_width = 320;
constexpr uint32_t g_source_screen_height = 200;
uint32_t g_screen_zoom = 1;

struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

void read_palette(Data data, Color palette[256]) {
  uint8_t* current = data.buffer;

  uint8_t start = *current++;
  uint8_t end = *current++;

  printf("Setting colors between %d and %d\n", start, end);

  memcpy(&palette[start], current, (end - start) * sizeof(Color));
}

struct ImageRowHeader {
  uint16_t length;
  uint16_t left;
  uint16_t top;
};

struct ImageHeader {
  uint16_t left;
  uint16_t top;
  uint16_t right;
  uint16_t bottom;
};

struct Image {
  uint16_t left;
  uint16_t top;
  uint16_t width;
  uint16_t height;
  Data pixels;
};

Image read_delt(Allocator* allocator, uint8_t* data, Color* palette) {
  ImageHeader* header = reinterpret_cast<ImageHeader*>(data);
  data += sizeof(ImageHeader);

  uint16_t image_width = header->right - header->left + 1;
  uint16_t image_height = header->bottom - header->top + 1;

  Data pixels = allocate(allocator, image_width * image_height * 4);
  uint8_t* buffer = pixels.buffer;
  memset(buffer, 0, image_width * image_height * 4);

  for (uint16_t y = 0; y < image_height - 1;) {
    uint16_t row_length = *(uint16_t*)data;
    data += 2;

    bool is_compressed = (row_length % 2) == 1;

    row_length >>= 1;

    if (row_length == 0) {
      return {};
    }

    uint16_t x = *(uint16_t*)data - header->left;
    data += 2;

    y = *(uint16_t*)data - header->top;
    data += 2;

    uint16_t start_x = x;
    uint16_t pitch = image_width * 4;

    for (; x < row_length + start_x;) {
      uint8_t b = *data++;

      if (((b % 2) == 1) && is_compressed) {
        // odd OP_CODE odd LENGTH = Repeat
        b >>= 1;  // number of repeats
        uint8_t p = *data++;
        for (size_t k = 0; k < b; ++k, ++x) {
          buffer[y * pitch + x * 4 + 0] = palette[p].red;
          buffer[y * pitch + x * 4 + 1] = palette[p].green;
          buffer[y * pitch + x * 4 + 2] = palette[p].blue;
          buffer[y * pitch + x * 4 + 3] = SDL_ALPHA_OPAQUE;
        }
      } else if (!is_compressed) {
        // Straight read
        --data;  // Reverse the op_code read.
        for (int k = 0; k < row_length; ++k, ++x) {
          uint8_t p = *data++;
          buffer[y * pitch + x * 4 + 0] = palette[p].red;
          buffer[y * pitch + x * 4 + 1] = palette[p].green;
          buffer[y * pitch + x * 4 + 2] = palette[p].blue;
          buffer[y * pitch + x * 4 + 3] = SDL_ALPHA_OPAQUE;
        }
      } else {
        // even OP_CODE = Read
        b >>= 1;
        for (int k = 0; k < b; ++k, ++x) {
          uint8_t p = *data++;
          buffer[y * pitch + x * 4 + 0] = palette[p].red;
          buffer[y * pitch + x * 4 + 1] = palette[p].green;
          buffer[y * pitch + x * 4 + 2] = palette[p].blue;
          buffer[y * pitch + x * 4 + 3] = SDL_ALPHA_OPAQUE;
        }
      }
    }
  }

  Image image{};
  image.left = header->left;
  image.top = header->top;
  image.width = image_width;
  image.height = image_height;
  image.pixels = pixels;

  return image;
}

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("[ERROR] %s\n", SDL_GetError());
    return -1;
  }

  SDL_Window* window = SDL_CreateWindow("red5", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        g_source_screen_width * g_screen_zoom,
                                        g_source_screen_height * g_screen_zoom, SDL_WINDOW_SHOWN);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

  Allocator global_allocator{};
  new_allocator(&global_allocator, 10 * 1024 * 1024);  // 10mb

  lfd::File lfd_file{};
  lfd::load_file(
      R"(C:\Program Files (x86)\Steam\steamapps\common\STAR WARS X-Wing\classic\RESOURCE\AWARDS.LFD)",
      &global_allocator, &lfd_file);

//  lfd::print_headers(&lfd_file);

  auto film_resource = lfd::get_resource(&lfd_file, "FILM", "award2_f");
  film::load(film_resource);

#if 0
  Color palette[256] = {};

  Data PLTTlogoname = lfd::get_resource(&lfd_file, "PLTT", "mainback");
  read_palette(PLTTlogoname, palette);

  //  Data PLTTlogoluke = lfd::get_resource(&lfd_file, "PLTT", "logoluke");
  //  read_palette(PLTTlogoluke, palette);
  //
  //  Data PLTTlogoman = lfd::get_resource(&lfd_file, "PLTT", "logoman");
  //  read_palette(PLTTlogoman, palette);

#define LOAD_TEXTURE(Name)                                                                         \
  Data Name = lfd::get_resource(&lfd_file, "DELT", #Name);                                         \
  Image img_##Name = read_delt(&global_allocator, (Name).buffer, palette);                         \
  SDL_Surface* surface_##Name = SDL_CreateRGBSurfaceWithFormatFrom(                                \
      img_##Name.pixels.buffer, img_##Name.width, img_##Name.height, 32, img_##Name.width * 4,     \
      SDL_PIXELFORMAT_RGBA32);                                                                     \
  SDL_Texture* texture_##Name = SDL_CreateTextureFromSurface(renderer, surface_##Name);            \
  SDL_FreeSurface(surface_##Name);                                                                 \
  deallocate(&global_allocator, &img_##Name.pixels)

  LOAD_TEXTURE(mainmenu);
  LOAD_TEXTURE(maindoor);
  LOAD_TEXTURE(maintitl);

#endif  // 0

  lfd::free_file(&lfd_file);

#if 0
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
          break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

#if 0
#define RENDER_TEXTURE(Name)                                                                       \
  SDL_Rect dst_##Name = {img_##Name.left * g_screen_zoom, img_##Name.top * g_screen_zoom,          \
                         img_##Name.width * g_screen_zoom, img_##Name.height * g_screen_zoom};     \
  SDL_RenderCopy(renderer, texture_##Name, nullptr, &dst_##Name)

    // SDL_RenderCopy(renderer, palette_texture, nullptr, nullptr);

    RENDER_TEXTURE(maindoor);
    RENDER_TEXTURE(mainmenu);
    RENDER_TEXTURE(maintitl);
#endif  // 0

    SDL_RenderPresent(renderer);
  }
#endif  // 0

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
