#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>
#include <renderer/sprite_renderer.h>

#include <vector>

#include "game/font.h"
#include "game/props.h"
#include "game/resources.h"
#include "game/scene_delegate.h"
#include "game/scene_renderer.h"

namespace game {

class Scene {
public:
  Scene(SceneDelegate* sceneDelegate, Resources* resources, SceneRenderer* sceneRenderer);

  PropContainer& props() {
    return m_props;
  }

  bool loadPalette(std::string_view name);
  bool loadFilm(std::string_view name);
  bool loadFont(std::string_view name);

  PropId insertImage(std::string_view name, std::vector<lfd::KeyFrame> keyFrames);
  PropId insertAnimation(std::string_view name, std::vector<lfd::KeyFrame> keyFrames);

  void update(U32 millis);
  void renderGameScreen();
  void renderDebugInfo();

private:
  void applyPalette(const Palette& palette);

  PropId insertImageProp(std::string_view name, const Image& image,
                         std::vector<lfd::KeyFrame> keyFrames);
  PropId insertAnimationProp(std::string_view name, const Animation& animation,
                             std::vector<lfd::KeyFrame> keyFrames);

  void processFilm();
  void processViewBlock(const lfd::Film::Block& block);
  void processPaletteBlock(const lfd::Film::Block& block);
  void processImageBlock(const lfd::Film::Block& block);
  void processAnimationBlock(const lfd::Film::Block& block);

  void advanceToNextFrame();

  SceneDelegate* m_delegate;
  Resources* m_resources;
  SceneRenderer* m_sceneRenderer;

  std::unique_ptr<lfd::Film> m_film;
  std::unique_ptr<Font> m_font;

  SDL_Color m_palette[256];

  PropContainer m_props;
  std::vector<PropId> m_renderOrder;

  U32 m_totalMillis = 0;
  I32 m_currentFrame = 0;
  U16 m_frameCount = 0;

  // Debug stuff:
  ResourceType m_selectedResourceType{};
  std::string m_selectedEntry;
};

}  // namespace game
