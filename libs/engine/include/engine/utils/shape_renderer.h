#pragma once

#include "engine/renderer/types.h"

namespace engine {

class Renderer;

class ShapeRenderer {
public:
  ShapeRenderer();

  void drawRectangle(const RectI& rect);

  bool attachToRenderer(Renderer* renderer);
  void detachFromRenderer();

private:
  Renderer* m_renderer;

  VertexBufferDefinition m_vertexBufferDefinition;
};

}  // namespace engine
