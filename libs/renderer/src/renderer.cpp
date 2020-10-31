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

  Size windowSize{};
  SDL_GetWindowSize(m_window, &windowSize.width, &windowSize.height);
  resize(windowSize);

  //  if (!createProgram(g_textureVertexShader, g_textureFragmentShader, &m_textureProgram.program))
  //  {
  //    return false;
  //  }

  return true;
}

void Renderer::resize(const Size& size) {
  m_windowRenderTarget.size = size;

  GL_CHECK_VOID(glViewport(0, 0, size.width, size.height), "Could not set renderer viewport.");

  spdlog::info("Resize renderer: ({}, {})", size.width, size.height);
}

RenderTargetData* Renderer::renderTarget() {
  if (!m_currentRenderTarget) {
    return &m_windowRenderTarget;
  }

  return m_renderTargets.getData(m_currentRenderTarget);
}

void Renderer::setRenderTarget(RenderTargetId renderTarget) {
  m_currentRenderTarget = renderTarget;

  auto* renderTargetData = m_renderTargets.getData(renderTarget);

  GL_CHECK_VOID(glBindFramebuffer(GL_FRAMEBUFFER, renderTargetData->framebuffer),
                "Could not bind frame buffer.");

  GL_CHECK_VOID(glViewport(0, 0, renderTargetData->size.width, renderTargetData->size.height),
                "Could not set render target viewport size.");

  //  if (renderTarget != RenderTargetId::invalidValue()) {
  //    m_textureProgram.viewMatrix = glm::ortho(0.0f,
  //    static_cast<F32>(renderTargetData->size.width),
  //                                             static_cast<F32>(renderTargetData->size.height),
  //                                             0.0f);
  //  }
}

void Renderer::clearRenderTarget() {
  m_currentRenderTarget = RenderTargetId::invalidValue();

  GL_CHECK_VOID(glBindFramebuffer(GL_FRAMEBUFFER, 0), "Could not unbind frame buffer.");

  GL_CHECK_VOID(glViewport(0, 0, m_windowRenderTarget.size.width, m_windowRenderTarget.size.height),
                "Could not set render target viewport size.");
}

void Renderer::renderRenderTarget(RenderTargetId renderTarget, const Rect& destination) {
  auto renderTargetData = m_renderTargets.getData(renderTarget);
  if (!renderTargetData) {
    spdlog::warn("Render target not found.");
    return;
  }

  GL_CHECK_VOID(glBindFramebuffer(GL_READ_FRAMEBUFFER, renderTargetData->framebuffer),
                "Could not bind read frame buffer.");
  GL_CHECK_VOID(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0), "Could not bind draw frame buffer.");
#if 0
  GL_CHECK_VOID(glBlitFramebuffer(0, 0, renderTargetData->size.width, renderTargetData->size.height,
                                  destination.position.left,
                                  m_windowRenderTarget.size.height - destination.size.height,
                                  destination.size.width, m_windowRenderTarget.size.height,
                                  GL_COLOR_BUFFER_BIT, GL_NEAREST),
                "Could not blit frame buffer.");
#endif
  GL_CHECK_VOID(glBlitFramebuffer(0, 0, renderTargetData->size.width, renderTargetData->size.height,
                                  destination.position.left, destination.position.top,
                                  destination.position.left + destination.size.width,
                                  destination.position.top + destination.size.height,
                                  GL_COLOR_BUFFER_BIT, GL_NEAREST),
                "Could not blit frame buffer.");
}

void Renderer::clear(F32 red, F32 green, F32 blue, F32 alpha) {
  GL_CHECK_VOID(glClearColor(red, green, blue, alpha), "Could not set clear color.")
  GL_CHECK_VOID(glClear(GL_COLOR_BUFFER_BIT), "Could not clear.")
}

void Renderer::beginFrame() {
  Size windowSize{};
  SDL_GetWindowSize(m_window, &windowSize.width, &windowSize.height);

  // spdlog::info("Window size: ({}, {})", windowSize.width, windowSize.height);

  GL_CHECK_VOID(glViewport(0, 0, windowSize.width, windowSize.height), "Could not set viewport.")
}

void Renderer::finishFrame() {
  SDL_GL_SwapWindow(m_window);
}

void Renderer::renderVertexBuffer(VertexBufferId vertexBuffer, ProgramId program, TextureId texture,
                                  const UniformData& uniformData) {
  auto vertexBufferData = m_vertexBuffers.getData(vertexBuffer);
  auto programData = m_programs.getData(program);
  auto textureData = m_textures.getData(texture);

  GL_CHECK_VOID(glUseProgram(programData->id), "Could not bind program.")
  GL_CHECK_VOID(glBindVertexArray(vertexBufferData->id), "Could not bind vertex array.")
  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_2D, textureData->texture), "Could not bind texture.")

  // Upload all the uniform data to the GPU.
  for (auto& uniform : uniformData.m_data) {
    U32 location = glGetUniformLocation(programData->id, uniform.name.data());
    if (!detail::clearGlError("Could not find uniform in program.", __FILE__, __LINE__)) {
      return;
    }

    // spdlog::info("Setting uniform ({})", uniform.name);

    switch (uniform.type) {
      case UniformType::Unknown:
        spdlog::error("Unknown uniform type.");
        return;

      case UniformType::Float1:
        GL_CHECK_VOID(glUniform1f(location, uniform.floatData[0]), "Could not set uniform data.");
        break;

      case UniformType::Float2:
        GL_CHECK_VOID(glUniform2f(location, uniform.floatData[0], uniform.floatData[1]),
                      "Could not set uniform data.");
        break;

      case UniformType::Float3:
        GL_CHECK_VOID(
            glUniform3f(location, uniform.floatData[0], uniform.floatData[1], uniform.floatData[2]),
            "Could not set uniform data.");
        break;

      case UniformType::Float4:
        GL_CHECK_VOID(glUniform4f(location, uniform.floatData[0], uniform.floatData[1],
                                  uniform.floatData[2], uniform.floatData[3]),
                      "Could not set uniform data.");
        break;

      case UniformType::Matrix4x4:
        GL_CHECK_VOID(glUniformMatrix4fv(location, 1, 0, uniform.floatData),
                      "Could not set uniform data.");
        break;

      case UniformType::Texture:
        GL_CHECK_VOID(glUniform1uiv(location, 1, uniform.unsignedIntData),
                      "Could not set uniform data.");
        break;
    };
  }

  GL_CHECK_VOID(glEnable(GL_BLEND), "Could not enable blending.");
  GL_CHECK_VOID(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                "Could not set blending function.");

  GL_CHECK_VOID(glDrawArrays(GL_TRIANGLES, 0, 6), "Could not draw arrays.")

  GL_CHECK_VOID(glDisable(GL_BLEND), "Could not disable blending.");

  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.")
  GL_CHECK_VOID(glBindVertexArray(0), "Could not unbind vertex array.")
  GL_CHECK_VOID(glUseProgram(0), "Could not unbind program.")
}

}  // namespace renderer
