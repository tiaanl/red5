#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <engine/utils/sprite_renderer.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>

#include <vector>

#include "game/font.h"
#include "game/props.h"
#include "game/resources.h"
#include "game/scene_delegate.h"
#include "game/scene_renderer.h"

#define DEBUG_UI 0

namespace game {

class Scene {
public:
  Scene(SceneDelegate* sceneDelegate, Resources* resources, SceneRenderer* sceneRenderer);

  PropContainer& props() {
    return m_props;
  }

  Prop* prop(PropId propId);

  bool loadPalette(std::string_view name);
  bool loadFilm(std::string_view name);
  bool loadFont(std::string_view name);

  PropId insertImage(std::string_view name, std::vector<lfd::KeyFrame> keyFrames = {});

  template <typename Func>
  PropId insertImage(std::string_view name, Func&& func) {
    auto propId = insertImage(name);
    if (propId.isValid()) {
      auto* prop = m_props.getData(propId);
      func(*prop);
    }

    return propId;
  }

  PropId insertAnimation(std::string_view name, std::vector<lfd::KeyFrame> keyFrames = {});

  template <typename Func>
  PropId insertAnimation(std::string_view name, Func&& func) {
    auto propId = insertAnimation(name);
    if (propId.isValid()) {
      auto* prop = m_props.getData(propId);
      func(*prop);
    }

    return propId;
  }

  PropId propId(std::string_view name);
  PropId propUnderMouse(const PositionI& position);

  void update(U32 millis);
  void renderGameScreen();
#if DEBUG_UI >= 0
  void renderDebugInfo();
#endif

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
