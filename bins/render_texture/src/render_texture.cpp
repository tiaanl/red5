
#include <engine/engine.h>
#include <glad/glad.h>

const char* g_vertexShader = R"(
#version 330 core

layout (location = 0) in vec2 vs_in_position;
layout (location = 1) in vec2 vs_in_texCoord;

out vec2 vs_out_texCoord;

void main()
{
    gl_Position = vec4(vs_in_position.xy, 0.0, 1.0);
    vs_out_texCoord = vs_in_texCoord;
}
)";

const char* g_fragmentShader = R"(
#version 330 core

in vec2 vs_out_texCoord;

out vec4 fs_out_fragColor;

uniform sampler2D u_texture;

void main()
{
  float c = texture(u_texture, vs_out_texCoord).r;
  fs_out_fragColor = vec4(c, c, c, 1.0f);
}
)";

class MyStage : public engine::Stage {
public:
  bool onLoad() override {
#if 0
    U8 data[] = {
        255, 0,   0,   255,  // red
        0,   255, 0,   255,  // green
        0,   0,   255, 255,  // blue
        255, 255, 255, 255,  // white
    };
#endif  // 0

#if 0
    renderer::Size size{9, 11};
    U8 data[] = {
        255, 255, 0,   0,   0,   0,   0,   0,   0,    //
        255, 127, 255, 0,   0,   0,   0,   0,   0,    //
        255, 127, 127, 255, 0,   0,   0,   0,   0,    //
        255, 127, 127, 127, 255, 0,   0,   0,   0,    //
        255, 127, 127, 127, 127, 255, 0,   0,   0,    //
        255, 127, 127, 127, 127, 127, 255, 0,   0,    //
        255, 127, 127, 127, 127, 127, 127, 255, 0,    //
        255, 127, 127, 127, 127, 127, 127, 127, 255,  //
        255, 127, 127, 127, 127, 255, 255, 255, 255,  //
        255, 127, 127, 255, 255, 0,   0,   0,   0,    //
        255, 255, 255, 0,   0,   0,   0,   0,   0,    //
    };
#endif

#if 0
    renderer::Size size{8, 8};
    U8 data[] = {
        255, 255, 255, 255, 255, 255, 255, 255,       //
        255, 127, 127, 127, 127, 127, 127, 255,       //
        255, 127, 127, 127, 127, 127, 127, 255,       //
        255, 127, 127, 127, 127, 127, 127, 255,       //
        255, 127, 127, 127, 127, 127, 127, 255,       //
        255, 127, 127, 127, 127, 127, 127, 255,       //
        255, 127, 127, 127, 127, 127, 127, 255,       //
        255, 255, 255, 255, 255, 255, 255, 255,       //
    };
#endif

#if 1
    renderer::Size size{5, 4};
    U8 data[] = {
        255, 255, 127, 255, 0,   0, 0, 0,  //
        0,   0,   255, 0,   255, 0, 0, 0,  //
        255, 255, 0,   255, 0,   0, 0, 0,  //
        0,   0,   255, 0,   255, 0, 0, 0,  //
    };
#endif  // 0

    U32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, size.width, size.height, 0, GL_RED, GL_UNSIGNED_BYTE,
                 data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    m_texture = m_renderer->textures().createFromRaw(texture, {2, 2});
    if (!m_texture.isValid()) {
      return false;
    }

    F32 vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,  //
        1.0f, 0.0f, 1.0f, 0.0f,  //
        1.0f, 1.0f, 1.0f, 1.0f,  //

        1.0f, 1.0f, 1.0f, 1.0f,  //
        0.0f, 1.0f, 0.0f, 1.0f,  //
        0.0f, 0.0f, 0.0f, 0.0f,  //
    };

    renderer::VertexBufferDefinition def;
    def.addAttribute(renderer::AttributeType::Float32, renderer::ComponentCount::Two);
    def.addAttribute(renderer::AttributeType::Float32, renderer::ComponentCount::Two);
    m_vertexBuffer = m_renderer->vertexBuffers().create(def, vertices, sizeof(vertices));
    if (!m_vertexBuffer) {
      return false;
    }

    m_program = m_renderer->programs().create(g_vertexShader, g_fragmentShader);
    if (!m_program) {
      return false;
    }

    return true;
  }

  void onUpdate(U32 millis) override {}

  void onRender() override {
    renderer::UniformData uniforms;
    uniforms.set("u_texture", m_texture);
    m_renderer->renderVertexBuffer(m_vertexBuffer, m_program, uniforms);
  }

private:
  renderer::ProgramId m_program;
  renderer::VertexBufferId m_vertexBuffer;
  renderer::TextureId m_texture;
};

int main(int argc, char** argv) {
  engine::Engine engine;
  if (!engine.init("Render Texture")) {
    return 1;
  }

  if (!engine.setStage(std::make_unique<MyStage>())) {
    return 1;
  }

  engine.run();

  return 0;
}
