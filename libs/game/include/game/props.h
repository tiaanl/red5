#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>
#include <renderer/sprite_renderer.h>
#include <renderer/types.h>

#include <memory>

#include "game/scene_delegate.h"
#include "game/timeline.h"

namespace game {

class SceneRenderer;

class Prop {
public:
  Prop(ResourceType type, std::string_view name, SceneDelegate* delegate, U32 frameCount,
       std::vector<lfd::KeyFrame> keyFrames, std::vector<renderer::Sprite> sprites);

  ResourceType resourceType() const {
    return m_resourceType;
  }

  std::string_view name() const {
    return m_name;
  }

  const Frame& currentFrame() const {
    return m_timeline.currentFrame();
  }

  void sceneTick(I32 sceneFrame);
  void render(SceneRenderer* renderer);

protected:
  ResourceType m_resourceType;
  std::string m_name;

  SceneDelegate* m_delegate;
  std::vector<lfd::KeyFrame> m_keyFrames;
  std::vector<renderer::Sprite> m_sprites;

  Timeline m_timeline;

#if 0
  bool m_visible = false;
  I16 m_currentSpriteIndex = 0;
  I16 m_layer = 0;
  Position m_offset = {0, 0};
  Position m_movePerFrame = {0, 0};
  Position m_orientation = {0, 0};

  struct AnimationState {
    I16 direction = 0;
    I16 frameRate = 0;
  } m_animation;
#endif  // 0

private:
  friend class Scene;
};

class PropContainer : public renderer::ResourceContainer<Prop> {
public:
  Identifier create(ResourceType type, std::string_view name, SceneDelegate* delegate,
                    U32 frameCount, std::vector<lfd::KeyFrame> keyFrames,
                    std::vector<renderer::Sprite> sprites);
};

using PropId = PropContainer::Identifier;

}  // namespace game
