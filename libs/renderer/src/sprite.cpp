#include "renderer/sprite.h"

namespace renderer {

Sprite::Sprite(TextureId texture, const Size& size) : m_texture{texture}, m_rect{{0, 0}, size} {}

Sprite::Sprite(TextureId texture, const Rect& rect) : m_texture{texture}, m_rect{rect} {}

void Sprite::setPosition(const Position& position) {
  m_rect.position = position;
}

void Sprite::setSize(const Size& size) {
  m_rect.size = size;
}

}  // namespace renderer
