#pragma once

#include <SDL2/SDL_render.h>
namespace renderer {

class RenderTarget {

private:
  SDL_Texture* m_texture;
};

}  // namespace renderer
