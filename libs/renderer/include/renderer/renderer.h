#pragma once

#include <SDL2/SDL_render.h>

#include <glm/mat4x4.hpp>

#include "renderer/dimensions.h"
#include "renderer/types.h"
#include "renderer/vertex_buffer_definition.h"
#include "renderer/uniform_data.h"

namespace renderer {

class Renderer {
public:
  bool init(SDL_Window* window);

  void resize(const Size& size);

  // Render target.

  RenderTargetData* renderTarget();
  void setRenderTarget(RenderTargetId renderTarget);
  void clearRenderTarget();
  void renderRenderTarget(RenderTargetId renderTarget, const Rect& destination);

  // Manage resources.

  RenderTargetContainer& renderTargets() {
    return m_renderTargets;
  }

  TextureContainer& textures() {
    return m_textures;
  }

  VertexBufferContainer& vertexBuffers() {
    return m_vertexBuffers;
  }

  ProgramContainer& programs() {
    return m_programs;
  }

  // Rendering.

  void clear(F32 red, F32 green, F32 blue, F32 alpha);
  void renderVertexBuffer(VertexBufferId vertexBuffer, ProgramId program, TextureId texture,
                          const UniformData& uniformData);

  // Per frame.

  void beginFrame();
  void finishFrame();

private:
  SDL_Window* m_window;

  RenderTargetData m_windowRenderTarget;
  RenderTargetId m_currentRenderTarget = RenderTargetId::invalidValue();

  RenderTargetContainer m_renderTargets;
  TextureContainer m_textures;
  VertexBufferContainer m_vertexBuffers;
  ProgramContainer m_programs;
};

}  // namespace renderer
