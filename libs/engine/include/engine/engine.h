#pragma once

#include <memory>

#include "engine/resources.h"
#include "engine/scene.h"
#include "engine/stage.h"
#include "true_type_font.h"

namespace engine {

class Engine {
public:
  Engine(Resources* resources, SDL_Renderer* renderer);

  SDL_Renderer* renderer() const {
    return m_renderer;
  }

  void setStage(std::unique_ptr<Stage> stage);

  void update(U32 ticks);
  void renderGameScreen();
  void renderOverlay();

private:
  Resources* m_resources;
  SDL_Renderer* m_renderer;

  TrueTypeFont m_ttf;
  std::unique_ptr<Stage> m_currentStage;
};

}  // namespace engine
