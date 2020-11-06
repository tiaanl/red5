#pragma once

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

  void render(const Sprite& sprite) const;

  virtual bool attachToRenderer(Renderer* renderer);
  virtual void detachFromRenderer();

protected:
  virtual ProgramId createProgramInternal();
  virtual void buildUniforms(const Sprite& sprite, UniformData* uniforms) const;
  void renderInternal(const Sprite& sprite, ProgramId program, UniformData uniforms) const;

  Renderer* m_renderer;

private:
  ProgramId m_program;
  VertexArrayId m_vertexBuffer;
};

}  // namespace renderer
