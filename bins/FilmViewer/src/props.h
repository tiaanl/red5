#pragma once

#include <SDL2/SDL_pixels.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>

#include <memory>

#include "base/platform.h"

struct RenderState {
  SDL_Color* palette;
  U16 screenWidth;
  U16 screenHeight;
  SDL_Color* pixels;
};

struct ScenePropState {
  U16 layer;
  bool visible;
};

class Prop {
public:
  struct Offset {
    I32 x = 0;
    I32 y = 0;
  };

  Prop(std::vector<Film::Chunk> chunks);

  U32 layer() const {
    return m_layer;
  }

  U32 visible() const {
    return m_visible;
  }

  virtual void nextFrame(U32 sceneFrame);
  virtual void render(const RenderState& renderState) = 0;

protected:
  void updateState(U32 sceneFrame);

  std::vector<Film::Chunk> m_chunks;

  Offset m_offset;
  U32 m_layer = 0;
  struct MovePerFrame {
    U32 x = 0;
    U32 y = 0;
  } m_movePerFrame;
  bool m_visible = false;
};

class ImageProp : public Prop {
public:
  ImageProp(std::vector<Film::Chunk> chunks, std::unique_ptr<Image> image);

  void render(const RenderState& renderState) override;

private:
  std::unique_ptr<Image> m_image;
};

class AnimationProp : public Prop {
public:
  AnimationProp(std::vector<Film::Chunk> chunks, std::unique_ptr<Animation> animation);

  void nextFrame(U32 sceneFrame) override;
  void render(const RenderState& renderState) override;

private:
  U32 m_currentFrame = 0;
  std::unique_ptr<Animation> m_animation;
};
