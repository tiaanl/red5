#pragma once

#include <memory>

#include "engine/resources.h"
#include "engine/scene.h"
#include "engine/stage.h"
#include "true_type_font.h"

namespace engine {

class Engine {
public:
  void addResourceFile(const ResourceFile& resourceFile);
  void setStage(std::unique_ptr<Stage> stage);

  // Lifecycle
  bool init(std::string_view windowTitle);
  void run();

private:
  void update(U32 ticks);
  void renderGameScreen();
  void renderOverlay();

  Resources m_resources;
  SDL_Window* m_window;
  SDL_Renderer* m_renderer;
  SDL_Texture* m_screen;

  TrueTypeFont m_ttf{20};
  std::unique_ptr<Stage> m_currentStage;
};

}  // namespace engine
