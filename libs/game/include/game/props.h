#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <engine/renderer/types.h>
#include <engine/utils/sprite_renderer.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>

#include <memory>

#include "game/scene_listener.h"
#include "game/timeline.h"

namespace game {

class SceneRenderer;

class Prop {
public:
  Prop(ResourceType type, std::string_view name, U32 frameCount,
       std::vector<lfd::KeyFrame> keyFrames, std::vector<engine::Sprite> sprites);

  ResourceType resourceType() const {
    return m_resourceType;
  }

  std::string_view name() const {
    return m_name;
  }

  PlaybackControls& playbackControls() {
    return m_playbackControls;
  }

  Frame& currentFrame() {
    return m_timeline.frame(m_playbackControls.currentFrame());
  }

  const Frame& currentFrame() const {
    return m_timeline.frame(m_playbackControls.currentFrame());
  }

  void setAnimate(bool animate);

  const PositionI& offset() const {
    return m_offset;
  }

  void moveTo(const PositionI& position);

  PlaybackControls& spritesPlaybackControls() {
    return m_spritesPlaybackControls;
  }
  const engine::Sprite& sprite(I16 index) const;

  RectI bounds() const;

  void sceneTick(I32 sceneFrame);
  void render(SceneRenderer* renderer) const;

protected:
  friend class Scene;

  ResourceType m_resourceType;
  std::string m_name;

  PositionI m_offset;
  std::vector<lfd::KeyFrame> m_keyFrames;

  bool m_animate = true;

  PlaybackControls m_spritesPlaybackControls;
  std::vector<engine::Sprite> m_sprites;

  PlaybackControls m_playbackControls;
  Timeline m_timeline;
};

class PropContainer : public engine::ResourceContainer<Prop> {
public:
  Identifier create(ResourceType type, std::string_view name, U32 frameCount,
                    std::vector<lfd::KeyFrame> keyFrames, std::vector<engine::Sprite> sprites);
};

using PropId = PropContainer::Identifier;

}  // namespace game
