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

  const renderer::Sprite& sprite(I16 index) const;
  const RectI& bounds() const;

  void sceneTick(I32 sceneFrame);
  void render(SceneRenderer* renderer) const;

protected:
  friend class Scene;

  ResourceType m_resourceType;
  std::string m_name;

  SceneDelegate* m_delegate;
  std::vector<lfd::KeyFrame> m_keyFrames;
  std::vector<renderer::Sprite> m_sprites;

  Timeline m_timeline;
};

class PropContainer : public renderer::ResourceContainer<Prop> {
public:
  Identifier create(ResourceType type, std::string_view name, SceneDelegate* delegate,
                    U32 frameCount, std::vector<lfd::KeyFrame> keyFrames,
                    std::vector<renderer::Sprite> sprites);
};

using PropId = PropContainer::Identifier;

}  // namespace game
