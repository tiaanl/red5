#include "renderer/sprite_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace renderer {

namespace {

const char* g_textureVertexShader = R"(
#version 330 core

layout (location = 0) in vec2 vs_in_position;
layout (location = 1) in vec2 vs_in_texCoord;

out vec2 vs_out_texCoord;

uniform vec4 u_rect;
uniform mat4 u_viewMatrix;

void main()
{
    // gl_Position = u_viewMatrix * vec4(vs_in_position, 0.0f, 1.0f);

    gl_Position = u_viewMatrix * vec4(
        u_rect.x + vs_in_position.x * u_rect.z,
        u_rect.y + vs_in_position.y * u_rect.w,
        0.0f,
        1.0f);

    // gl_Position = vec4(vs_in_position, 0.0f, 1.0f);

    vs_out_texCoord = vs_in_texCoord;
}
)";

const char* g_textureFragmentShader = R"(
#version 330 core

in vec2 vs_out_texCoord;

out vec4 fs_out_fragColor;

uniform sampler2D u_texture;

void main()
{
  fs_out_fragColor = texture(u_texture, vs_out_texCoord);
}
)";

}  // namespace

SpriteRenderer::~SpriteRenderer() {
  detachFromRenderer();
}

void SpriteRenderer::render(const Sprite& sprite) const {
  UniformData uniforms;
  buildUniforms(sprite, &uniforms);
  renderInternal(sprite, m_program, uniforms);
}

bool SpriteRenderer::attachToRenderer(Renderer* renderer) {
  m_renderer = renderer;

  VertexBufferDefinition def;
  def.addAttribute(AttributeType::Float32, ComponentCount::Two);  // 2D position
  def.addAttribute(AttributeType::Float32, ComponentCount::Two);  // Texture coordinate

  static F32 vertices[] = {
      0.0f, 0.0f, 0.0f, 0.0f,  //
      1.0f, 0.0f, 1.0f, 0.0f,  //
      1.0f, 1.0f, 1.0f, 1.0f,  //

      1.0f, 1.0f, 1.0f, 1.0f,  //
      0.0f, 1.0f, 0.0f, 1.0f,  //
      0.0f, 0.0f, 0.0f, 0.0f,  //
  };

  m_vertexBuffer = m_renderer->vertexBuffers().create(def, vertices, sizeof(vertices));
  if (!m_vertexBuffer) {
    return false;
  }

  m_program = createProgramInternal();
  if (!m_program) {
    return false;
  }

  return true;
}

void SpriteRenderer::detachFromRenderer() {
  m_renderer->programs().destroy(m_program);
  m_renderer->vertexBuffers().destroy(m_vertexBuffer);

  m_renderer = nullptr;
}

ProgramId SpriteRenderer::createProgramInternal() {
  return m_renderer->programs().create(g_textureVertexShader, g_textureFragmentShader);
}

void SpriteRenderer::buildUniforms(const Sprite& sprite, UniformData* uniforms) const {
  auto targetSize = m_renderer->renderTarget()->size;
  auto viewMatrix = glm::ortho(0.0f, static_cast<F32>(targetSize.width),
                               static_cast<F32>(targetSize.height), 0.0f);

  auto rect = sprite.m_rect;
  uniforms->set("u_viewMatrix", viewMatrix);
  uniforms->set("u_rect", static_cast<F32>(rect.position.left), static_cast<F32>(rect.position.top),
                static_cast<F32>(rect.size.width), static_cast<F32>(rect.size.height));
  uniforms->set("u_texture", sprite.m_texture);
}

void SpriteRenderer::renderInternal(const Sprite& sprite, ProgramId program,
                                    const UniformData& uniforms) const {
  m_renderer->renderVertexBuffer(m_vertexBuffer, program, sprite.m_texture, uniforms);
}

}  // namespace renderer
