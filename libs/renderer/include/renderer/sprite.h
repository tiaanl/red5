#pragma once

#include "renderer/dimensions.h"
#include "renderer/types.h"
#include "renderer/renderer.h"

namespace renderer {

class Sprite {
public:
  Sprite(TextureId textureId, const Rect& rect);

  const Rect& rect() const {
    return m_rect;
  }

private:
  friend class SpriteRenderer;

  TextureId m_texture;
  Rect m_rect;
};

}  // namespace renderer
