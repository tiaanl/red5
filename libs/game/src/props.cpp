#include "game/props.h"

#include <game/scene_renderer.h>

#define TRACE_OP_CODES 0

namespace game {

Prop::Prop(ResourceType type, std::string_view name, SceneDelegate* delegate, U32 frameCount,
           std::vector<lfd::KeyFrame> keyFrames, std::vector<renderer::Sprite> sprites)
  : m_resourceType{type},
    m_name{name},
    m_delegate{delegate},
    m_keyFrames{std::move(keyFrames)},
    m_sprites{std::move(sprites)} {
  m_timeline.build(frameCount, m_keyFrames);
}

const renderer::Sprite& Prop::sprite(I16 index) const {
  index = std::min<I16>(static_cast<I16>(m_sprites.size()), index);
  index = std::max<I16>(0, index);
  return m_sprites[index];
}

const Rect& Prop::bounds() const {
  return sprite(currentFrame().spriteIndex).rect();
}

void Prop::sceneTick(I32 sceneFrame) {
  m_timeline.setCurrentFrame(sceneFrame);
}

void Prop::render(SceneRenderer* renderer) const {
  auto frame = currentFrame();

  if (frame.visible) {
    auto sprite = this->sprite(frame.spriteIndex);
    sprite.setPosition(sprite.position() + frame.offset);
    renderer->render(sprite);
  }
}

renderer::ResourceContainer<Prop>::Identifier PropContainer::create(
    ResourceType type, std::string_view name, SceneDelegate* delegate, U32 frameCount,
    std::vector<lfd::KeyFrame> keyFrames, std::vector<renderer::Sprite> sprites) {
  return emplaceData(type, name, delegate, frameCount, std::move(keyFrames), std::move(sprites));
}

}  // namespace game
