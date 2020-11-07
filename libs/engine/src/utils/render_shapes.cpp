#include "engine/utils/render_shapes.h"

#include "engine/renderer/renderer.h"

namespace engine {

void MeshBuilder::submit(Renderer& renderer) {
  assert(!m_vertices.empty());  // Submitting empty mesh.

  renderer.renderImmediate(RenderMode::TriangleStrip, m_vertices.data(),
                           static_cast<U32>(m_vertices.size()));
}

MeshBuilder::MeshBuilder(RenderMode renderMode) : m_renderMode{renderMode} {}

void renderRectangle(Renderer& renderer, const RectF& rect, const ColorF& color) {
  auto mb = MeshBuilder::create(RenderMode::TriangleStrip);
  mb.vertex()
      .position(rect.position.left, rect.position.top)
      .texCoord(0.0f, 0.0f)
      .color(color)
      .build();
  mb.vertex()
      .position(rect.position.left + rect.size.width, rect.position.top)
      .texCoord(1.0f, 0.0f)
      .color(color)
      .build();
  mb.vertex()
      .position(rect.position.left, rect.position.top + rect.size.height)
      .texCoord(0.0f, 1.0f)
      .color(color)
      .build();
  mb.vertex()
      .position(rect.position.left + rect.size.width, rect.position.top + rect.size.height)
      .texCoord(1.0f, 1.0f)
      .color(color)
      .build();
  mb.submit(renderer);
}

void renderRectangleOutline(Renderer& renderer, const RectF& rect, const ColorF& color) {
  auto mb = MeshBuilder::create(RenderMode::TriangleStrip);
  mb.vertex()
      .position(rect.position.left, rect.position.top)
      .texCoord(0.0f, 0.0f)
      .color(color)
      .build();
  mb.vertex()
      .position(rect.position.left + rect.size.width, rect.position.top)
      .texCoord(1.0f, 0.0f)
      .color(color)
      .build();
  mb.vertex()
      .position(rect.position.left + rect.size.width, rect.position.top + rect.size.height)
      .texCoord(1.0f, 1.0f)
      .color(color)
      .build();
  mb.vertex()
      .position(rect.position.left, rect.position.top + rect.size.height)
      .texCoord(0.0f, 1.0f)
      .color(color)
      .build();
  mb.submit(renderer);
}

}  // namespace engine
