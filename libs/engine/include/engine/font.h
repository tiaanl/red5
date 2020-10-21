#pragma once

#include <SDL2/SDL_render.h>
#include <lfd/font.h>

namespace engine {

class Font {
public:
  ~Font();

  void renderText(SDL_Renderer* renderer, const SDL_Point& position, std::string_view text);

  bool load(SDL_Renderer* renderer, const lfd::Font& font);

private:
  struct Glyph {
    U16 width;
    SDL_Texture* texture;
  };

  U16 m_height = 0;
  U16 m_baseLine = 0;
  Glyph m_glyphs[256] = {};
};

}  // namespace engine
