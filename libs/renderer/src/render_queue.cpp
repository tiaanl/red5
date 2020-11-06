#include "renderer/render_queue.h"

#include <glad/glad.h>

#include "gl_check.h"
#include "renderer/renderer.h"

namespace renderer {

RenderQueue::RenderQueue(const RenderTargetData& windowRenderTarget)
  : m_windowRenderTarget{windowRenderTarget} {}

void RenderQueue::clearRenderTarget(RenderTargetId renderTarget, const ColorF& color) {
  auto& command = m_renderCommands.emplace_back(RenderCommandType::ClearRenderTarget);
  command.clearRenderTarget.renderTarget = renderTarget;
  command.clearRenderTarget.color = color;
}

void RenderQueue::copyRenderTarget(RenderTargetId from, const RectI& fromRect, RenderTargetId to,
                                   const RectI& toRect) {
  auto& command = m_renderCommands.emplace_back(RenderCommandType::CopyRenderTarget);
  command.copyRenderTarget.from = from;
  command.copyRenderTarget.fromRect = fromRect;
  command.copyRenderTarget.to = to;
  command.copyRenderTarget.toRect = toRect;
}

void RenderQueue::renderVertexBuffer(RenderTargetId renderTarget, VertexArrayId vertexBuffer,
                                     ProgramId program, UniformData uniformData) {
  auto& command = m_renderCommands.emplace_back(RenderCommandType::RenderVertexBuffer);
  command.renderVertexBuffer.renderTarget = renderTarget;
  command.renderVertexBuffer.vertexBuffer = vertexBuffer;
  command.renderVertexBuffer.program = program;
  command.renderVertexBuffer.uniformData = std::move(uniformData);
}

void RenderQueue::renderImmediate(RenderTargetId renderTarget, RenderMode renderMode, U32 startIndex, U32 count) {
  auto& command = m_renderCommands.emplace_back(RenderCommandType::RenderImmediate);
  command.renderImmediate.renderTarget = renderTarget;
  command.renderImmediate.renderMode = renderMode;
  command.renderImmediate.startIndex = startIndex;
  command.renderImmediate.count = count;
}

std::vector<RenderCommand> RenderQueue::flush() {
  return std::move(m_renderCommands);
}

}  // namespace renderer
