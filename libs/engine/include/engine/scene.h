#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>
#include <renderer/renderer.h>

#include <vector>

#include "engine/font.h"
#include "engine/props.h"
#include "engine/resources.h"
#include "engine/scene_delegate.h"

namespace engine {

class Scene {
public:
  explicit Scene(SceneDelegate* sceneDelegate, Resources* resources, renderer::Renderer* renderer);

  bool loadPalette(std::string_view name);
  bool loadFont(std::string_view name);
  bool loadFilm(std::string_view name);

  void update(U32 millis);
  void render();

private:
  void applyPalette(const Palette& palette);

  void processFilm();
  void processViewBlock(const Film::Block& block);
  void processPaletteBlock(const Film::Block& block);
  void processImageBlock(const Film::Block& block);
  void processAnimationBlock(const Film::Block& block);

  SceneDelegate* m_delegate;
  Resources* m_resources;
  renderer::Renderer* m_renderer;

  std::unique_ptr<Film> m_film;
  std::unique_ptr<Font> m_font;

  SDL_Color m_palette[256];

  std::vector<Prop> m_props;

  U32 m_totalMillis = 0;
  U32 m_currentFrame = 0;
  U16 m_frameCount = 0;
};

}  // namespace engine
