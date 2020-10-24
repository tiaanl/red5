#include "engine/true_type_font.h"

#define STB_TRUETYPE_IMPLEMENTATION

#include "stb/stb_truetype.h"

extern const unsigned char roboto_condensed_regular[];

namespace engine {

TrueTypeFont::TrueTypeFont(I32 size) : m_size{size} {}

void TrueTypeFont::loadFromFont(SDL_Renderer* renderer) {
  constexpr U32 w = 256;
  constexpr U32 h = 256;

  stbtt_fontinfo fontInto;
  stbtt_InitFont(&fontInto, roboto_condensed_regular, 0);
  stbtt_GetFontVMetrics(&fontInto, &m_ascent, &m_descent, &m_lineGap);

  std::vector<U8> pixels;
  pixels.resize(w * h);
  stbtt_bakedchar charData[96];
  stbtt_BakeFontBitmap(roboto_condensed_regular, 0, static_cast<F32>(m_size), pixels.data(), w, h,
                       32, 96, charData);

  for (U32 i = 0; i < 96; ++i) {
    m_glyphs[i].boundingBox = {charData[i].x0, charData[i].y0, charData[i].x1 - charData[i].x0,
                               charData[i].y1 - charData[i].y0};
    m_glyphs[i].xOffset = charData[i].xoff;
    m_glyphs[i].yOffset = charData[i].yoff;
    m_glyphs[i].xAdvance = charData[i].xadvance;
  }

  SDL_Surface* surface =
      SDL_CreateRGBSurfaceWithFormatFrom(pixels.data(), w, h, 8, w, SDL_PIXELFORMAT_INDEX8);
  if (!surface) {
    spdlog::error("Error: {}", SDL_GetError());
  }

  SDL_Palette* palette = SDL_AllocPalette(256);

  SDL_Color colors[256];
  for (U32 i = 0; i < 256; ++i) {
    colors[i].r = colors[i].g = colors[i].b = 255;
    colors[i].a = i;
  }
  SDL_SetPaletteColors(palette, colors, 0, 256);
  SDL_SetSurfacePalette(surface, palette);

  SDL_Surface* fullColorSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

  SDL_FreeSurface(surface);

  m_texture = SDL_CreateTextureFromSurface(renderer, fullColorSurface);
  if (!m_texture) {
    spdlog::error("Error: {}", SDL_GetError());
  }

  SDL_FreeSurface(surface);
}

void TrueTypeFont::renderText(SDL_Renderer* renderer, SDL_Point position,
                              std::string_view text) const {
  F32 x = static_cast<F32>(position.x);
  F32 y = static_cast<F32>(position.y + m_size);

  for (auto ch : text) {
    const auto& glyph = m_glyphs[ch - 32];

#if 0
    spdlog::info("boundingBox: ({}, {}, {}, {})", glyph.boundingBox.x, glyph.boundingBox.y,
                 glyph.boundingBox.w, glyph.boundingBox.w);
#endif  // 0

    SDL_Rect destination{static_cast<I32>(std::round(x + glyph.xOffset)),
                         static_cast<I32>(std::round(y + glyph.yOffset)), glyph.boundingBox.w,
                         glyph.boundingBox.h};

    SDL_RenderCopy(renderer, m_texture, &glyph.boundingBox, &destination);

    x += glyph.xAdvance;
  }
}

}  // namespace engine
