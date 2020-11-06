#include "renderer/renderer.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace renderer {

bool Renderer::init(SDL_Window* window) {
  m_window = window;

  if (!gladLoadGL()) {
    spdlog::critical("Could not load OpenGL interface.");
    return false;
  }

  SizeI windowSize{};
  SDL_GetWindowSize(m_window, &windowSize.width, &windowSize.height);
  resize(windowSize);

  return true;
}

void Renderer::resize(const SizeI& size) {
  m_windowRenderTarget.size = size;
}

RenderTargetData* Renderer::renderTarget() {
  if (!m_currentRenderTarget) {
    return &m_windowRenderTarget;
  }

  return m_renderTargets.getData(m_currentRenderTarget);
}

void Renderer::setRenderTarget(RenderTargetId renderTarget) {
  m_currentRenderTarget = renderTarget;
}

void Renderer::clearRenderTarget() {
  m_currentRenderTarget = RenderTargetId::invalidValue();
}

void Renderer::copyRenderTarget(const RectI& toRect, RenderTargetId from, const RectI& fromRect) {
  m_renderQueue.copyRenderTarget(from, fromRect, m_currentRenderTarget, toRect);
}

void Renderer::clear(F32 red, F32 green, F32 blue, F32 alpha) {
  m_renderQueue.clearRenderTarget(m_currentRenderTarget, {red, green, blue, alpha});
}

void Renderer::renderVertexBuffer(VertexBufferId vertexBuffer, ProgramId program,
                                  UniformData uniformData) {
  m_renderQueue.renderVertexBuffer(m_currentRenderTarget, vertexBuffer, program,
                                   std::move(uniformData));
}

void Renderer::flushRenderQueue() {
  m_renderQueue.flush(*this);
}

void Renderer::finishFrame() {
  SDL_GL_SwapWindow(m_window);
}

}  // namespace renderer
