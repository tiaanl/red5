#pragma once

#include "renderer/dimensions.h"
#include "renderer/renderer.h"
#include "renderer/types.h"

namespace renderer {

class Sprite {
public:
  Sprite(TextureId texture, const Size& size);
  Sprite(TextureId texture, const Rect& rect);

  const Position& position() const {
    return m_rect.position;
  }

  void setPosition(const Position& position);

  const Size& getSize() const {
    return m_rect.size;
  }

  void setSize(const Size& size);

  const Rect& rect() const {
    return m_rect;
  }

private:
  friend class SpriteRenderer;

  TextureId m_texture;
  Rect m_rect;
};

}  // namespace renderer
