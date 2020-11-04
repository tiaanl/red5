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

void Prop::sceneTick(I32 sceneFrame) {
  m_timeline.setCurrentFrame(sceneFrame);
}

void Prop::render(SceneRenderer* renderer) {
  auto frame = currentFrame();

  if (frame.visible) {
    auto spriteIndex = frame.spriteIndex;
    if (spriteIndex >= m_sprites.size()) {
      spriteIndex = static_cast<U32>(m_sprites.size()) - 1;
    } else if (spriteIndex < 0) {
      spriteIndex = 0;
    }
    auto sprite = m_sprites[spriteIndex];
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
