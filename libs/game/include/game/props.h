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

namespace game {

class SceneRenderer;

class Prop {
public:
  Prop(ResourceType type, std::string_view name, SceneDelegate* delegate,
       std::vector<Film::Chunk> chunks, std::vector<renderer::Sprite> sprites,
       std::vector<renderer::Sprite> indexSprites);

  ResourceType resourceType() const {
    return m_resourceType;
  }

  std::string_view name() const {
    return m_name;
  }

  I16 spriteIndex() const {
    return m_currentSpriteIndex;
  }

  void setSpriteIndex(I16 currentFrame);

  I16 layer() const {
    return m_layer;
  }

  void setLayer(I16 layer);

  const renderer::Position& offset() const {
    return m_offset;
  }

  void setOffset(const renderer::Position& offset);

  void sceneTick(I32 sceneFrame);
  void render(SceneRenderer* renderer);

protected:
  void updateState(U32 sceneFrame);

  void applyMove(I16 x, I16 y, I16 xx, I16 yy);
  void applySpeed(I16 x, I16 y, I16 xx, I16 yy);
  void applyLayer(I16 layer);
  void applyFrame(I16 frame, I16 x);
  void applyAnimation(I16 direction, I16 frameRate);
  void applyEvent(I16 event);
  void applyWindow(I16 x, I16 y, I16 w, I16 h);
  void applyShift(I16 x, I16 y, I16 xx, I16 yy);
  void applyDisplay(I16 visible);
  void applyOrientation(I16 x, I16 y);

  ResourceType m_resourceType;
  std::string m_name;

  SceneDelegate* m_delegate;
  std::vector<Film::Chunk> m_chunks;
  std::vector<renderer::Sprite> m_sprites;
  std::vector<renderer::Sprite> m_indexSprites;

  bool m_visible = false;
  I16 m_currentSpriteIndex = 0;
  I16 m_layer = 0;
  renderer::Position m_offset = {0, 0};
  renderer::Position m_movePerFrame = {0, 0};
  renderer::Position m_orientation = {0, 0};

  struct AnimationState {
    I16 direction = 0;
    I16 frameRate = 0;
  } m_animation;

private:
  friend class Scene;
};

class PropContainer : public renderer::ResourceContainer<Prop> {
public:
  Identifier create(ResourceType type, std::string_view name, SceneDelegate* delegate,
                    std::vector<Film::Chunk> chunks, std::vector<renderer::Sprite> sprites,
                    std::vector<renderer::Sprite> indexSprites);
};

using PropId = PropContainer::Identifier;

}  // namespace game
