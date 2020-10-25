#pragma once

#include <SDL2/SDL_render.h>

#include "renderer/dimensions.h"

namespace renderer {

#define DECLARE_RESOURCE_ID(Resource)                                                              \
  struct Resource {                                                                                \
    U32 id;                                                                                        \
    static Resource invalidValue() {                                                               \
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

  // Render target.

  void setRenderTarget(RenderTargetId renderTarget);
  void clearRenderTarget();

  // Manage resources.

  TextureId createTexture(void* data, I32 width, I32 height);
  RenderTargetId createRenderTarget(I32 width, I32 height);

  // Rendering.

  void clear(F32 red, F32 green, F32 blue, F32 alpha);
  void copyTexture(TextureId texture, const Rect& to);
  void copyTexture(TextureId texture, const Rect& from, const Rect& to);

  void copyTexture(RenderTargetId renderTarget, const Rect& to);

  // Per frame.

  void present();

private:
  struct TextureData {
    SDL_Texture* texture;
    Size size;

    TextureData(SDL_Texture* texture, const Size& size) : texture{texture}, size{size} {}
  };

  struct RenderTargetData {
    SDL_Texture* texture;
    Size size;

    RenderTargetData(SDL_Texture* texture, const Size& size) : texture{texture}, size{size} {}
  };

  void copyTextureInternal(SDL_Texture* texture, const Rect& from, const Rect& to);

  SDL_Renderer* m_sdl;
  RenderTargetId m_renderTarget;

  std::vector<TextureData> m_textures;
  std::vector<RenderTargetData> m_renderTargets;
};

}  // namespace renderer
