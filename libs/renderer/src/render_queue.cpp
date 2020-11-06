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

void RenderQueue::renderVertexBuffer(RenderTargetId renderTarget, VertexBufferId vertexBuffer,
                                     ProgramId program, UniformData uniformData) {
  auto& command = m_renderCommands.emplace_back(RenderCommandType::RenderVertexBuffer);
  command.renderVertexBuffer.renderTarget = renderTarget;
  command.renderVertexBuffer.vertexBuffer = vertexBuffer;
  command.renderVertexBuffer.program = program;
  command.renderVertexBuffer.uniformData = std::move(uniformData);
}

std::vector<RenderCommand> RenderQueue::flush() {
  return std::move(m_renderCommands);
}

}  // namespace renderer
