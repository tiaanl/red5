#pragma once

#include <engine/utils/sprite_renderer.h>
#include <lfd/font.h>

namespace game {

class Font {
public:
  ~Font();

  void renderText(engine::SpriteRenderer* renderer, const PositionI& position,
                  std::string_view text);

  bool load(engine::Renderer* renderer, const lfd::Font& font);

private:
  struct Glyph {
    U16 width;
    engine::TextureId texture;
  };

  U16 m_height = 0;
  U16 m_baseLine = 0;
  Glyph m_glyphs[256] = {};
};

}  // namespace game
