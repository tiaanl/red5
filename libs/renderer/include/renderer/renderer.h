#pragma once

#include <SDL2/SDL_render.h>

#include <glm/mat4x4.hpp>

#include "renderer/render_queue.h"
#include "renderer/types.h"
#include "renderer/uniform_data.h"
#include "renderer/vertex_buffer_definition.h"

namespace renderer {

class Renderer {
public:
  bool init(SDL_Window* window);

  void resize(const SizeI& size);

  // Render target.

  RenderTargetData* renderTarget();
  void setRenderTarget(RenderTargetId renderTarget);
  void clearRenderTarget();
  void copyRenderTarget(const RectI& toRect, RenderTargetId from, const RectI& fromRect);

  // Manage resources.

  RenderTargetContainer& renderTargets() {
    return m_renderTargets;
  }

  TextureContainer& textures() {
    return m_textures;
  }

  VertexArrayContainer& vertexBuffers() {
    return m_vertexArrays;
  }

  ProgramContainer& programs() {
    return m_programs;
  }

  // Rendering.

  void clear(F32 red, F32 green, F32 blue, F32 alpha);
  void renderVertexBuffer(VertexArrayId vertexBuffer, ProgramId program, UniformData uniformData);
  void renderImmediate(RenderMode renderMode, ImmediateVertex* vertices, U32 count);

  // Per frame.

  void flushRenderQueue();
  void finishFrame();

private:
  bool initImmediateMode();
  void switchRenderTarget(RenderTargetId renderTarget);

  void renderImmediateCommand(const RenderCommand::RenderImmediate& command);

  SDL_Window* m_window;

  RenderTargetData m_windowRenderTarget;
  RenderTargetId m_currentRenderTarget = RenderTargetId::invalidValue();
  RenderTargetId m_lastRenderTarget;

  RenderTargetContainer m_renderTargets;
  TextureContainer m_textures;
  VertexArrayContainer m_vertexArrays;
  ProgramContainer m_programs;

  struct ImmediateMode {
    ProgramId programId;
    VertexArrayId vertexArray;
    std::vector<ImmediateVertex> vertices;
  } m_immediateMode;

  RenderQueue m_renderQueue{m_windowRenderTarget};
};

}  // namespace renderer
