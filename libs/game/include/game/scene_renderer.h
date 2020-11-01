#pragma once

#include <lfd/palette.h>
#include <renderer/color.h>
#include <renderer/renderer.h>
#include <renderer/sprite_renderer.h>

namespace game {

class SceneRenderer : public renderer::SpriteRenderer {
public:
  SceneRenderer();

  void setPalette(const renderer::RGB* colors, U8 startIndex, U8 endIndex);

  // Override: renderer::SpriteRenderer
  bool attachToRenderer(renderer::Renderer* renderer) override;
  void detachFromRenderer() override;

protected:
  // Override: renderer::SpriteRenderer
  renderer::ProgramId createProgramInternal() override;
  void buildUniforms(const renderer::Sprite& sprite,
                     renderer::UniformData* uniforms) const override;

private:
  renderer::TextureId m_paletteTexture;
};

}  // namespace game
