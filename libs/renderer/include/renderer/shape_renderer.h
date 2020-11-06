#pragma once

#include <optional>

#include "renderer/types.h"

namespace renderer {

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

}  // namespace renderer
