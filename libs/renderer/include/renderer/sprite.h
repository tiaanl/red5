#pragma once

#include "renderer/renderer.h"
#include "renderer/types.h"

namespace renderer {

class Sprite {
public:
  Sprite(TextureId texture, const SizeI& size);
  Sprite(TextureId texture, const RectI& rect);

  const PositionI& position() const {
    return m_rect.position;
  }

  void setPosition(const PositionI& position);

  const SizeI& getSize() const {
    return m_rect.size;
  }

  void setSize(const SizeI& size);

  const RectI& rect() const {
    return m_rect;
  }

private:
  friend class SpriteRenderer;

  TextureId m_texture;
  RectI m_rect;
};

}  // namespace renderer
