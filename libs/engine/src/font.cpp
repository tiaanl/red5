#include "engine/font.h"

namespace engine {

namespace {

bool isBitSet(U8 byte, U8 bit) {
  return (byte & (1u << bit)) == (1u << bit);
}

}  // namespace

Font::~Font() {
  for (auto& glyph : m_glyphs) {
    if (glyph.texture) {
      SDL_DestroyTexture(glyph.texture);
    }
  }
}

void Font::renderText(SDL_Renderer* renderer, const SDL_Point& position, std::string_view text) {
  SDL_Rect s{0, 0, 0, m_height};
  SDL_Rect d{position.x, position.y, 0, m_height};

  for (U8 ch : text) {
    auto& glyph = m_glyphs[ch];

    SDL_Texture* texture = glyph.texture;
    if (texture == nullptr) {
      continue;
    }

    U16 width = glyph.width;
    s.w = width;
    d.w = width;

    SDL_RenderCopy(renderer, texture, &s, &d);

    d.x += width + 1;
  }
}

bool Font::load(SDL_Renderer* renderer, const lfd::Font& font) {
  m_height = font.height();
  m_baseLine = font.baseLine();

  for (MemSize i = 0; i < font.glyphs().size(); ++i) {
    const auto& glyph = font.glyphs()[i];

    std::vector<SDL_Color> image;
    image.resize(font.height() * glyph.width);

    for (U16 y = 0; y < font.height(); ++y) {
      for (U8 x = 0; x < glyph.width; ++x) {
        auto pos = y * glyph.width + x;

        U8 offset = x - (x - (x % 8));
        auto bytePos = y * (font.bitsPerLine() / 8) + (offset / 8);

        U8 byte = glyph.data[bytePos];
        auto isSet = isBitSet(byte, 7 - offset);

        if (isSet) {
          image[pos] = SDL_Color{255, 255, 255, 255};
        } else {
          image[pos] = SDL_Color{0, 0, 0, 0};
        }
      }
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(
        image.data(), glyph.width, font.height(), 32,
        static_cast<int>(glyph.width * sizeof(SDL_Color)), SDL_PIXELFORMAT_RGBA32);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    m_glyphs[font.startChar() + i].width = glyph.width;
    m_glyphs[font.startChar() + i].texture = texture;

    SDL_FreeSurface(surface);
  }

  return true;
}

}  // namespace engine
