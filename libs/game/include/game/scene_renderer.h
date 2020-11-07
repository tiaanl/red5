#pragma once

#include <engine/renderer/color.h>
#include <engine/renderer/renderer.h>
#include <engine/utils/sprite_renderer.h>
#include <lfd/palette.h>

namespace game {

class SceneRenderer : public engine::SpriteRenderer {
public:
  SceneRenderer();

  void setPalette(const engine::RGB* colors, U8 startIndex, U8 endIndex);

  // Override: engine::SpriteRenderer
  bool attachToRenderer(engine::Renderer* renderer) override;
  void detachFromRenderer() override;

protected:
  // Override: engine::SpriteRenderer
  engine::ProgramId createProgramInternal() override;
  void buildUniforms(const engine::Sprite& sprite, engine::UniformData* uniforms) const override;

private:
  engine::TextureId m_paletteTexture;
};

}  // namespace game
