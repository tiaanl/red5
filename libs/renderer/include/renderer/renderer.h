#pragma once

#include <SDL2/SDL_render.h>

#include <glm/mat4x4.hpp>

#include "renderer/dimensions.h"

namespace renderer {

#define DECLARE_RESOURCE_ID(Resource)                                                              \
  struct Resource {                                                                                \
    U32 id;                                                                                        \
    constexpr static Resource invalidValue() {                                                     \
      return {std::numeric_limits<U32>::max()};                                                    \
    }                                                                                              \
    friend bool operator==(const Resource& left, const Resource& right) {                          \
      return left.id == right.id;                                                                  \
    }                                                                                              \
    friend bool operator!=(const Resource& left, const Resource& right) {                          \
      return left.id != right.id;                                                                  \
    }                                                                                              \
    bool operator!() const {                                                                       \
      return id == invalidValue().id;                                                              \
    }                                                                                              \
  }

DECLARE_RESOURCE_ID(TextureId);
DECLARE_RESOURCE_ID(RenderTargetId);

#undef DECLARE_RESOURCE_ID

class Renderer {
public:
  bool init(SDL_Window* window);

  void resize(const Size& size);

  // Render target.

  void setRenderTarget(RenderTargetId renderTarget);
  void clearRenderTarget();

  // Manage resources.

  TextureId createTexture(void* data, const Size& size);
  RenderTargetId createRenderTarget(const Size& size);

  // Rendering.

  void clear(F32 red, F32 green, F32 blue, F32 alpha);
  void copyTexture(TextureId texture, const Rect& to);
  void copyTexture(TextureId texture, const Rect& from, const Rect& to);

  void copyTexture(RenderTargetId renderTarget, const Rect& to);

  // Per frame.
  void beginFrame();
  void finishFrame();

private:
  struct TextureData {
    U32 texture;
    Size size;

    TextureData(U32 texture, const Size& size) : texture{texture}, size{size} {}
  };

  struct RenderTargetData {
    U32 framebuffer;
    U32 texture;
    Size size;

    RenderTargetData(U32 frameBuffer, U32 texture, const Size& size)
      : framebuffer{frameBuffer}, texture{texture}, size{size} {}
  };

  void copyTextureInternal(U32 texture, const Rect& from, const Rect& to);

  SDL_Window* m_window;

  Size m_windowSize;

  RenderTargetId m_currentRenderTarget = RenderTargetId::invalidValue();

  struct TextureProgram {
    U32 program = 0;
    U32 vertexBuffer = 0;
    U32 vertexArrayObject = 0;
    glm::mat4 viewMatrix;
  } m_textureProgram;

  std::vector<TextureData> m_textures;
  std::vector<RenderTargetData> m_renderTargets;
};

}  // namespace renderer
