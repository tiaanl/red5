#include "game/props.h"

#include <game/scene_renderer.h>

#define TRACE_OP_CODES 0

namespace game {

Prop::Prop(ResourceType type, std::string_view name, U32 frameCount,
           std::vector<lfd::KeyFrame> keyFrames, std::vector<engine::Sprite> sprites)
  : m_resourceType{type},
    m_name{name},
    m_keyFrames{std::move(keyFrames)},
    m_spritesPlaybackControls{0, static_cast<I32>(sprites.size())},
    m_sprites{std::move(sprites)},
    m_playbackControls{0, static_cast<I32>(frameCount)} {
  m_timeline.build(frameCount, m_keyFrames);
}

void Prop::setAnimate(bool animate) {
  m_animate = animate;
}

void Prop::moveTo(const PositionI& position) {
  m_offset = position;
}

const engine::Sprite& Prop::sprite(I16 index) const {
  index = std::min<I16>(static_cast<I16>(m_sprites.size()), index);
  index = std::max<I16>(0, index);
  return m_sprites[index];
}

RectI Prop::bounds() const {
  const auto& frame = currentFrame();
  auto spriteIndex = m_animate ? frame.spriteIndex : m_spritesPlaybackControls.currentFrame();
  return sprite(spriteIndex).rect() + frame.offset + m_offset;
}

void Prop::sceneTick(I32 sceneFrame) {
  if (m_animate) {
    m_playbackControls.jumpToFrame(sceneFrame);
  } else {
    m_spritesPlaybackControls.nextFrame();
  }
}

void Prop::render(SceneRenderer* renderer) const {
  auto frame = currentFrame();

  if (frame.visible) {
    // If we're animating, then we follow the timeline for sprite indices.
    auto spriteIndex = m_animate ? frame.spriteIndex : m_spritesPlaybackControls.currentFrame();

    auto sprite = this->sprite(spriteIndex);
    sprite.setPosition(bounds().position);
    renderer->render(sprite);
  }
}

engine::ResourceContainer<Prop>::Identifier PropContainer::create(
    ResourceType type, std::string_view name, U32 frameCount, std::vector<lfd::KeyFrame> keyFrames,
    std::vector<engine::Sprite> sprites) {
  return emplaceData(type, name, frameCount, std::move(keyFrames), std::move(sprites));
}

}  // namespace game
