#include "renderer/renderer.h"

namespace renderer {

void renderRectangle(Renderer& renderer, const RectF& rect, const ColorF& color) {
  F32 x1 = rect.position.left;
  F32 y1 = rect.position.top;
  F32 x2 = rect.position.left + rect.size.width;
  F32 y2 = rect.position.top + rect.size.height;
  ImmediateVertex vertices[] = {
      {{x1, y1}, color},
      {{x2, y1}, color},
      {{x1, y2}, color},
      {{x2, y2}, color},
  };

  renderer.renderImmediate(RenderMode::TriangleStrip, vertices, 4);
}

void renderRectangleOutline(Renderer& renderer, const RectF& rect, const ColorF& color) {
  F32 x1 = rect.position.left;
  F32 y1 = rect.position.top;
  F32 x2 = rect.position.left + rect.size.width;
  F32 y2 = rect.position.top + rect.size.height;
  ImmediateVertex vertices[] = {
      {{x1, y1}, color},
      {{x2, y1}, color},
      {{x2, y2}, color},
      {{x1, y2}, color},
  };

  renderer.renderImmediate(RenderMode::LineLoop, vertices, 4);
}

}  // namespace renderer
