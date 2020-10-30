#include "renderer/sprite.h"

namespace renderer {

Sprite::Sprite(TextureId texture, const Size& size)
  : m_texture{texture}, m_size{size} {}

}  // namespace renderer
