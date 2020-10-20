#pragma once

#include <memory>

#include "engine/scene.h"
#include "engine/stage.h"
#include "engine/resources.h"

namespace engine {

class Engine {
public:
  Engine(Resources* resources, SDL_Renderer* renderer);

  SDL_Renderer* renderer() const {
    return m_renderer;
  }

  void setStage(std::unique_ptr<Stage> stage);

  void update(U32 ticks);
  void render();

private:
  Resources* m_resources;
  SDL_Renderer* m_renderer;

  std::unique_ptr<Stage> m_currentStage;
};

}  // namespace engine
