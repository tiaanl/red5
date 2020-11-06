#include "renderer/shape_renderer.h"

#include "renderer/renderer.h"

namespace renderer {

ShapeRenderer::ShapeRenderer() {
  // Position
  m_vertexBufferDefinition.addAttribute(AttributeType::Float32, ComponentCount::Two);
  // Color
  m_vertexBufferDefinition.addAttribute(AttributeType::Float32, ComponentCount::Two);
}

void ShapeRenderer::drawRectangle(const RectI& rect) {
  if (!m_renderer) {
    return;
  }

  F32 vertices[][4] = {
      {0.0f, 0.0f, static_cast<F32>(rect.position.left), static_cast<F32>(rect.position.top)},
      {0.0f, 0.0f, static_cast<F32>(rect.position.left), static_cast<F32>(rect.position.top)},
  };

  m_renderer->vertexBuffers().create(m_vertexBufferDefinition, vertices, sizeof(vertices));
}

bool ShapeRenderer::attachToRenderer(Renderer* renderer) {
  m_renderer = renderer;

  return true;
}
void ShapeRenderer::detachFromRenderer() {
  m_renderer = nullptr;
}

}  // namespace renderer
