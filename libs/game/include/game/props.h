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

class RenderItem {
public:
  RenderItem(renderer::TextureId texture, const renderer::Rect& rect);

  RenderItem(const RenderItem&) = delete;
  RenderItem(RenderItem&& other) : m_texture{other.m_texture}, m_rect{other.m_rect} {
    other.m_texture = renderer::TextureId::invalidValue();
  }

  ~RenderItem();

  RenderItem& operator=(const RenderItem&) = delete;
  RenderItem& operator=(RenderItem&& other) {
    m_texture = other.m_texture;
    m_rect = other.m_rect;

    other.m_texture = renderer::TextureId::invalidValue();

    return *this;
  };

  bool render(renderer::SpriteRenderer* renderer, const renderer::Position& offset,
              const renderer::Position& orientation);

private:
  renderer::TextureId m_texture;
  renderer::Rect m_rect;
};

class Prop {
public:
  Prop(SceneDelegate* delegate, std::vector<Film::Chunk> chunks,
       std::vector<RenderItem> renderItems);

  U32 layer() const {
    return m_layer;
  }

  void nextFrame(U32 sceneFrame);
  void render(renderer::SpriteRenderer* renderer);

protected:
  void updateState(U32 sceneFrame);
  void resetState();

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

  SceneDelegate* m_delegate;
  std::vector<Film::Chunk> m_chunks;
  std::vector<RenderItem> m_renderItems;

  bool m_visible = false;
  I16 m_currentFrame = 0;
  I16 m_layer = 0;
  renderer::Position m_offset = {0, 0};
  renderer::Position m_movePerFrame = {0, 0};
  renderer::Position m_orientation = {0, 0};

  struct AnimationState {
    I16 direction = 0;
    I16 frameRate = 0;
  } m_animation;
};

}  // namespace game
