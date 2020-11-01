#include "game/scene_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace game {

namespace {

const char* g_vertexShader = R"(
#version 330 core

layout (location = 0) in vec2 vs_in_position;
layout (location = 1) in vec2 vs_in_texCoord;

out vec2 vs_out_texCoord;

uniform vec4 u_rect;
uniform mat4 u_viewMatrix;

void main()
{
    gl_Position = u_viewMatrix * vec4(
        u_rect.x + vs_in_position.x * u_rect.z,
        u_rect.y + vs_in_position.y * u_rect.w,
        0.0f,
        1.0f);

    vs_out_texCoord = vs_in_texCoord;
}
)";

const char* g_fragmentShader = R"(
#version 330 core

in vec2 vs_out_texCoord;

out vec4 fs_out_fragColor;

uniform sampler2D u_texture;
uniform sampler2D u_palette;

void main()
{
  // float index = texture(u_texture, vs_out_texCoord).r;
  // vec4 color = texture(u_palette, vec2(index, 0.0f));
  vec4 color = texture(u_palette, vec2(0.1f, 0.0f));
  fs_out_fragColor = color;
}
)";

}  // namespace

SceneRenderer::SceneRenderer() = default;

void SceneRenderer::setPalette(const renderer::RGB* colors, U8 startIndex, U8 endIndex) {
  m_renderer->textures().update1D(m_paletteTexture, colors, startIndex, endIndex);
}

bool SceneRenderer::attachToRenderer(renderer::Renderer* renderer) {
  renderer::SpriteRenderer::attachToRenderer(renderer);

  renderer::RGB colors[256] = {};
  m_paletteTexture = m_renderer->textures().create1D(colors, 256);
  if (!m_paletteTexture) {
    return false;
  }

  return true;
}

void SceneRenderer::detachFromRenderer() {
  m_renderer->textures().destroy(m_paletteTexture);

  SpriteRenderer::detachFromRenderer();
}

renderer::ProgramId SceneRenderer::createProgramInternal() {
  return m_renderer->programs().create(g_vertexShader, g_fragmentShader);
}

void SceneRenderer::buildUniforms(const renderer::Sprite& sprite,
                                  renderer::UniformData* uniforms) const {
  SpriteRenderer::buildUniforms(sprite, uniforms);
  uniforms->set("u_palette", m_paletteTexture);
}

}  // namespace game
