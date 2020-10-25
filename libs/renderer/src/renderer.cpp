#include "renderer/renderer.h"

#include <SDL2/SDL.h>

namespace renderer {

namespace {

template <typename IdType, typename DataType, typename... Args>
IdType addData(std::vector<DataType>* vector, Args&&... args) {
  vector->push_back(DataType{std::forward<Args>(args)...});
  return static_cast<IdType>(IdType{static_cast<U32>(vector->size() - 1)});
}

template <typename DataType, typename IdType>
const DataType* getData(const std::vector<DataType>& vector, IdType id) {
  return &vector[id.id];
}

U8 floatColorToByte(F32 color) {
  return static_cast<U8>(std::round(255.0f * color));
}

}  // namespace

bool Renderer::init(SDL_Window* window) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return false;
  }

  m_sdl = SDL_CreateRenderer(window, -1, 0);

  return true;
}

void Renderer::setRenderTarget(RenderTargetId renderTarget) {
  m_renderTarget = renderTarget;

  auto* renderTargetData = getData(m_renderTargets, renderTarget);
  SDL_SetRenderTarget(m_sdl, renderTargetData->texture);
}

void Renderer::clearRenderTarget() {
  m_renderTarget = std::numeric_limits<decltype(m_renderTarget)>::max();

  SDL_SetRenderTarget(m_sdl, nullptr);
}

TextureId Renderer::createTexture(void* data, I32 width, I32 height) {
  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(data, width, height, 32, width * 4,
                                                            SDL_PIXELFORMAT_RGBA32);

  SDL_Texture* texture = SDL_CreateTextureFromSurface(m_sdl, surface);

  SDL_FreeSurface(surface);

  m_textures.emplace_back(texture, Size{width, height});
  return {static_cast<U32>(m_textures.size() - 1)};
}

RenderTargetId Renderer::createRenderTarget(I32 width, I32 height) {
  SDL_Texture* texture =
      SDL_CreateTexture(m_sdl, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);

  return addData<RenderTargetId>(&m_renderTargets, texture, Size{width, height});
}

void Renderer::clear(F32 red, F32 green, F32 blue, F32 alpha) {
  SDL_SetRenderDrawColor(m_sdl, floatColorToByte(red), floatColorToByte(green),
                         floatColorToByte(blue), floatColorToByte(alpha));
  SDL_RenderClear(m_sdl);
}

void Renderer::copyTexture(TextureId texture, const Rect& to) {
  auto textureData = getData(m_textures, texture);
  if (!textureData) {
    spdlog::warn("Rendering texture that does not exist.");
    return;
  }

  Rect from{{0, 0}, textureData->size};
  copyTextureInternal(textureData->texture, from, to);
}

void Renderer::copyTexture(TextureId texture, const Rect& from, const Rect& to) {
  auto textureData = getData(m_textures, texture);
  if (!textureData) {
    spdlog::warn("Rendering texture that does not exist.");
    return;
  }

  copyTextureInternal(textureData->texture, from, to);
}

void Renderer::copyTexture(RenderTargetId renderTarget, const Rect& to) {
  auto renderTargetData = getData(m_renderTargets, renderTarget);
  if (!renderTargetData) {
    spdlog::warn("Rendering texture that does not exist.");
    return;
  }

  Rect from{{0, 0}, renderTargetData->size};
  copyTextureInternal(renderTargetData->texture, from, to);
}

void Renderer::present() {
  SDL_RenderPresent(m_sdl);
}

void Renderer::copyTextureInternal(SDL_Texture* texture, const Rect& from, const Rect& to) {
#if 0
  spdlog::info("copyTextureInternal: ({}, {}, {}, {}) to ({}, {}, {}, {})",               //
               from.position.left, from.position.top, from.size.width, from.size.height,  //
               to.position.left, to.position.top, to.size.width, to.size.height);
#endif  // 0

  SDL_Rect source{from.position.left, from.position.top, from.size.width, from.size.height};
  SDL_Rect destination{to.position.left, to.position.top, to.size.width, to.size.height};
  SDL_RenderCopy(m_sdl, texture, &source, &destination);
}

}  // namespace renderer
