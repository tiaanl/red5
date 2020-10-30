#pragma once

#include "renderer/dimensions.h"
#include "renderer/types.h"
#include "renderer/renderer.h"

namespace renderer {

class Sprite {
public:
  Sprite(TextureId textureId, const Size& size);

  const Size& size() const {
    return m_size;
  }

private:
  friend class SpriteRenderer;

  TextureId m_texture;
  Size m_size;
};

}  // namespace renderer
