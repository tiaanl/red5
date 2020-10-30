#include "engine/font.h"

#include <renderer/renderer.h>
#include <renderer/sprite_renderer.h>

namespace engine {

namespace {

bool isBitSet(U8 byte, U8 bit) {
  return (byte & (1u << bit)) == (1u << bit);
}

}  // namespace

Font::~Font() {
  // TODO: Destroy all the textures.
}

void Font::renderText(renderer::SpriteRenderer* renderer, const renderer::Position& position,
                      std::string_view text) {
  SDL_Rect s{0, 0, 0, m_height};
  SDL_Rect d{position.left, position.top, 0, m_height};

  for (U8 ch : text) {
    auto& glyph = m_glyphs[ch];

    auto texture = glyph.texture;

    U16 width = glyph.width;
    s.w = width;
    d.w = width;

    // renderer->copyTexture(texture, {d.x, d.y, d.w, d.h});
    renderer::Sprite sprite{texture, {d.w, d.h}};
    renderer->render({d.x, d.y}, sprite);

    // SDL_RenderCopy(renderer, texture, &s, &d);

    d.x += width + 1;
  }
}

bool Font::load(renderer::Renderer* renderer, const lfd::Font& font) {
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

    auto texture = renderer->textures().create(image.data(), {glyph.width, font.height()});
    m_glyphs[font.startChar() + i].width = glyph.width;
    m_glyphs[font.startChar() + i].texture = texture;
  }

  return true;
}

}  // namespace engine
