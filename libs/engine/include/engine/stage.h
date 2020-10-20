#pragma once

#include <SDL2/SDL_render.h>

#include "engine/resources.h"

namespace engine {

class Stage {
public:
  virtual ~Stage();

  virtual bool onReady() = 0;
  virtual void onUpdate(U32 millis) = 0;
  virtual void onRender() = 0;
};

}  // namespace engine
