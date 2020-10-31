#include "renderer/sprite.h"

namespace renderer {

Sprite::Sprite(TextureId texture, const Rect& rect)
  : m_texture{texture}, m_rect{rect} {}

}  // namespace renderer
