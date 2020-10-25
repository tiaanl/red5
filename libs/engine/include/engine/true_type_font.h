#pragma once

#include <renderer/renderer.h>

namespace engine {

class TrueTypeFont {
public:
  explicit TrueTypeFont(I32 size);

  void loadFromFont(renderer::Renderer* renderer);

  void renderText(renderer::Renderer* renderer, SDL_Point position, std::string_view text) const;

private:
  struct Glyph {
    SDL_Rect boundingBox;
    F32 xOffset;
    F32 yOffset;
    F32 xAdvance;
  };

  I32 m_size;
  SDL_Texture* m_texture = nullptr;
  I32 m_ascent = 0;
  I32 m_descent = 0;
  I32 m_lineGap = 0;
  Glyph m_glyphs[96] = {};
};

}  // namespace engine
