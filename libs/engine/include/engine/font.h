#pragma once

#include <lfd/font.h>
#include <renderer/renderer.h>

namespace engine {

class Font {
public:
  ~Font();

  void renderText(renderer::Renderer* renderer, const SDL_Point& position, std::string_view text);

  bool load(renderer::Renderer* renderer, const lfd::Font& font);

private:
  struct Glyph {
    U16 width;
    renderer::TextureId texture;
  };

  U16 m_height = 0;
  U16 m_baseLine = 0;
  Glyph m_glyphs[256] = {};
};

}  // namespace engine
