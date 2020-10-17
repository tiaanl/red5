#pragma once

#include <SDL2/SDL_pixels.h>
#include <lfd/film.h>
#include <lfd/image.h>
#include <lfd/resource_file.h>

#include <vector>

struct RenderState {
  SDL_Color* palette;
  U16 screenWidth;
  U16 screenHeight;
  SDL_Color* pixels;
};

struct ScenePropState {
  U16 layer;
  bool visible;
};

class SceneProp {
public:
  virtual void render(const RenderState& renderState) = 0;
};

class ImageSceneProp : public SceneProp {
public:
  ImageSceneProp(std::unique_ptr<Image> image);

  void render(const RenderState& renderState) override;

private:
  std::unique_ptr<Image> m_image;
};

class Scene {
public:
  Scene();

  void addResources(const ResourceFile& resourceFile);
  bool loadFilm(std::string_view name);
  void render(SDL_Color* pixels);

private:
  ResourceEntry* findResource(ResourceType resourceType, std::string_view name);

  void processFilm();
  void processViewBlock(const Film::Block& block);
  void processPaletteBlock(const Film::Block& block);
  void processImageBlock(const Film::Block& block);

  std::vector<ResourceEntry> m_resources;
  std::unique_ptr<Film> m_film;
  U16 m_width = 320;
  U16 m_height = 200;
  SDL_Color m_palette[256];

  std::vector<std::unique_ptr<SceneProp>> m_props;
};
