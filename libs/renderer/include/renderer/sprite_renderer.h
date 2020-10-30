#pragma once

#include "renderer/dimensions.h"
#include "renderer/sprite.h"
#include "renderer/types.h"

namespace renderer {

class Renderer;

class SpriteRenderer {
public:
  ~SpriteRenderer();

  Renderer* renderer() const {
    return m_renderer;
  }

  bool attachToRenderer(Renderer* renderer);
  void detachFromRenderer();

  // Render sprites.

  void render(const Position& position, const Sprite& sprite) const;

private:
  Renderer* m_renderer;

  VertexBufferId m_vertexBuffer;
  ProgramId m_program;
};

}  // namespace renderer
