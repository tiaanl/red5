#include "renderer/renderer.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace renderer {

namespace {

void processClearRenderTarget(Renderer& renderer, const RenderCommand& command) {
  GL_CHECK_VOID(
      glClearColor(command.clearRenderTarget.color.red, command.clearRenderTarget.color.green,
                   command.clearRenderTarget.color.blue, command.clearRenderTarget.color.alpha),
      "Could not set clear color.")
  GL_CHECK_VOID(glClear(GL_COLOR_BUFFER_BIT), "Could not clear.")
}

void processCopyRenderTarget(Renderer& renderer, const RenderCommand& command) {
  auto data = command.copyRenderTarget;

  U32 fromId = 0;
  if (data.from.isValid()) {
    auto renderTargetData = renderer.renderTargets().getData(data.from);
    if (!renderTargetData) {
      spdlog::warn("From flush target not found.");
      return;
    }

    fromId = renderTargetData->framebuffer;
  }

  GL_CHECK_VOID(glBindFramebuffer(GL_READ_FRAMEBUFFER, fromId),
                "Could not bind read frame buffer.");

  U32 toId = 0;
  if (data.to.isValid()) {
    auto renderTargetData = renderer.renderTargets().getData(data.to);
    if (!renderTargetData) {
      spdlog::warn("To flush target not found.");
      return;
    }

    toId = renderTargetData->framebuffer;
  }

  GL_CHECK_VOID(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, toId), "Could not bind draw frame buffer.");

  GL_CHECK_VOID(glBlitFramebuffer(data.fromRect.position.left, data.fromRect.position.top,  //
                                  data.fromRect.size.width, data.fromRect.size.height,      //
                                  data.toRect.position.left, data.toRect.position.top,      //
                                  data.toRect.size.width, data.toRect.size.height,          //
                                  GL_COLOR_BUFFER_BIT, GL_NEAREST),
                "Could not blit frame buffer.");
}

void processRenderVertexBuffer(Renderer& renderer, const RenderCommand& command) {
  auto& data = command.renderVertexBuffer;

  auto vertexBufferData = renderer.vertexBuffers().getData(data.vertexBuffer);
  auto programData = renderer.programs().getData(data.program);

  GL_CHECK_VOID(glUseProgram(programData->id), "Could not bind program.")
  GL_CHECK_VOID(glBindVertexArray(vertexBufferData->id), "Could not bind vertex array.")

  U32 nextTextureUnit = 0;

  // Upload all the uniform data to the GPU.
  for (const auto& uniform : data.uniformData) {
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

      case UniformType::Matrix4X4:
        GL_CHECK_VOID(glUniformMatrix4fv(location, 1, 0, uniform.floatData),
                      "Could not set uniform data.");
        break;

      case UniformType::Texture: {
        auto textureData = renderer.textures().getData(uniform.texture);
        if (!textureData) {
          spdlog::error("Texture from uniform not found.");
          break;
        }

        GL_CHECK_VOID(glActiveTexture(GL_TEXTURE0 + nextTextureUnit),
                      "Could not set active texture unit.");
        GL_CHECK_VOID(glBindTexture(textureData->dimensions, textureData->texture),
                      "Could not bind texture.");
        GL_CHECK_VOID(glUniform1i(location, nextTextureUnit), "Could not set texture uniform.");
        ++nextTextureUnit;
        break;
      }
    };
  }

  GL_CHECK_VOID(glDrawArrays(GL_TRIANGLES, 0, 6), "Could not draw arrays.")

  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.")
  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_1D, 0), "Could not unbind texture.")
  GL_CHECK_VOID(glBindVertexArray(0), "Could not unbind vertex array.")
  GL_CHECK_VOID(glUseProgram(0), "Could not unbind program.")
}

}  // namespace

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
  GL_CHECK_VOID(glEnable(GL_BLEND), "Could not enable blending.");
  GL_CHECK_VOID(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                "Could not set blending function.");

  for (const auto& command : m_renderQueue.flush()) {
    switch (command.type) {
      case RenderCommandType::ClearRenderTarget: {
        switchRenderTarget(command.clearRenderTarget.renderTarget);
        processClearRenderTarget(*this, command);
        break;
      }

      case RenderCommandType::CopyRenderTarget: {
        processCopyRenderTarget(*this, command);
        break;
      }

      case RenderCommandType::RenderVertexBuffer: {
        switchRenderTarget(command.renderVertexBuffer.renderTarget);
        processRenderVertexBuffer(*this, command);
        break;
      }
    }
  }

  GL_CHECK_VOID(glDisable(GL_BLEND), "Could not disable blending.");
}

void Renderer::finishFrame() {
  SDL_GL_SwapWindow(m_window);
}

void Renderer::switchRenderTarget(RenderTargetId renderTarget) {
  if (renderTarget != m_lastRenderTarget) {
    if (renderTarget.isValid()) {
      auto* renderTargetData = m_renderTargets.getData(renderTarget);

      GL_CHECK_VOID(glBindFramebuffer(GL_FRAMEBUFFER, renderTargetData->framebuffer),
                    "Could not bind frame buffer.");

      GL_CHECK_VOID(glViewport(0, 0, renderTargetData->size.width, renderTargetData->size.height),
                    "Could not set renderInternal target viewport size.");
    } else {
      GL_CHECK_VOID(glBindFramebuffer(GL_FRAMEBUFFER, 0), "Could not clear frame buffer.");

      GL_CHECK_VOID(
          glViewport(0, 0, m_windowRenderTarget.size.width, m_windowRenderTarget.size.height),
          "Could not set renderInternal target viewport size.");
    }

    m_lastRenderTarget = renderTarget;
  }
}

}  // namespace renderer
