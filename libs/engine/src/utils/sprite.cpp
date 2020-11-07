#include "engine/utils/sprite.h"

namespace engine {

Sprite::Sprite(TextureId texture, const SizeI& size) : m_texture{texture}, m_rect{{0, 0}, size} {}

Sprite::Sprite(TextureId texture, const RectI& rect) : m_texture{texture}, m_rect{rect} {}

void Sprite::setPosition(const PositionI& position) {
  m_rect.position = position;
}

void Sprite::setSize(const SizeI& size) {
  m_rect.size = size;
}

}  // namespace engine
