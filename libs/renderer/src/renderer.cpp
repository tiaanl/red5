#include "renderer/renderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    gl_Position = u_viewMatrix * vec4(
      u_rect.x + vs_in_position.x * u_rect.z,
      u_rect.y + vs_in_position.y * u_rect.w,
      0.0f, 1.0f);

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

template <typename IdType, typename DataType, typename... Args>
IdType addData(std::vector<DataType>* vector, Args&&... args) {
  vector->push_back(DataType{std::forward<Args>(args)...});
  return static_cast<IdType>(IdType{static_cast<U32>(vector->size() - 1)});
}

template <typename DataType, typename IdType>
const DataType* getData(const std::vector<DataType>& vector, IdType id) {
  if (id == IdType::invalidValue()) {
    return nullptr;
  }

  return &vector[id.id];
}

const char* openGlErrorToString(U32 error) {
  switch (error) {
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";

    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE";

    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION";

    default:
      return "Unknown";
  }
}

bool clearGlError(std::string_view failureMessage) {
  auto error = glad_glGetError();
  if (error == GL_NO_ERROR) {
    return true;
  }

  spdlog::error("{} ({})", failureMessage, openGlErrorToString(error));

  return false;
}

#define GL_CHECK(Code, FailureMessage, FailReturn)                                                 \
  {                                                                                                \
    glad_##Code;                                                                                   \
    if (!clearGlError(FailureMessage)) {                                                           \
      return (FailReturn);                                                                         \
    }                                                                                              \
  }

#define GL_CHECK_VOID(Code, FailureMessage)                                                        \
  {                                                                                                \
    glad_##Code;                                                                                   \
    if (!clearGlError(FailureMessage)) {                                                           \
      return;                                                                                      \
    }                                                                                              \
  }

bool createShader(U32 type, const char* source, U32* shader) {
  U32 name = glad_glCreateShader(type);
  if (!clearGlError("Could not create vertex shader.")) {
    return false;
  }

  I32 length = static_cast<I32>(std::strlen(source));
  glad_glShaderSource(name, 1, &source, &length);
  if (!clearGlError("Could not set shader source.")) {
    return false;
  }

  glad_glCompileShader(name);
  if (!clearGlError("Could not compile shader program.")) {
    return false;
  }

  I32 success;
  glad_glGetShaderiv(name, GL_COMPILE_STATUS, &success);
  if (!clearGlError("Could not get shader success status.")) {
    return false;
  }

  if (!success) {
    char log[1024];
    I32 infoLogLength;
    glad_glGetShaderInfoLog(name, 1024, &infoLogLength, log);
    if (!clearGlError("Could not get program info log.")) {
      return false;
    }
    log[length] = '\0';
    spdlog::warn("Could not compile shader program.\n{}", log);
    return false;
  }

  *shader = name;

  return true;
}

bool createProgram(const char* vertexShaderSource, const char* fragmentShaderSource, U32* program) {
  U32 vertexShader, fragmentShader;

  if (!createShader(GL_VERTEX_SHADER, vertexShaderSource, &vertexShader)) {
    return false;
  }

  if (!createShader(GL_FRAGMENT_SHADER, fragmentShaderSource, &fragmentShader)) {
    return false;
  }

  U32 programName = glad_glCreateProgram();
  if (!clearGlError("Could not create new program.")) {
    return false;
  }

  glad_glAttachShader(programName, vertexShader);
  if (!clearGlError("Could not attach vertex shader to program.")) {
    return false;
  }

  glad_glAttachShader(programName, fragmentShader);
  if (!clearGlError("Could not attach fragment shader to program.")) {
    return false;
  }

  glad_glLinkProgram(programName);
  if (!clearGlError("Could not link program.")) {
    return false;
  }

  glad_glDeleteShader(vertexShader);
  if (!clearGlError("Could not delete shader.")) {
    return false;
  }

  glad_glDeleteShader(fragmentShader);
  if (!clearGlError("Could not delete shader.")) {
    return false;
  }

  *program = programName;

  return true;
}

bool createVertexBuffer(const void* data, MemSize dataSize, U32* vertexBuffer) {
  U32 buffer;
  glad_glGenBuffers(1, &buffer);
  if (glad_glGetError() != GL_NO_ERROR) {
    spdlog::warn("Could not create array buffer.");
    return false;
  }

  glad_glBindBuffer(GL_ARRAY_BUFFER, buffer);
  if (glad_glGetError() != GL_NO_ERROR) {
    spdlog::warn("Could not bind array buffer.");
    return false;
  }

  glad_glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
  if (glad_glGetError() != GL_NO_ERROR) {
    spdlog::warn("Could not set array buffer data.");
    return false;
  }

  glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (!clearGlError("Could not unbind vertex buffer.")) {
    return false;
  }

  *vertexBuffer = buffer;

  return true;
}

}  // namespace

bool Renderer::init(SDL_Window* window) {
  m_window = window;

  if (!gladLoadGL()) {
    spdlog::critical("Could not load OpenGL interface.");
    return false;
  }

  Size windowSize{};
  SDL_GetWindowSize(m_window, &windowSize.width, &windowSize.height);
  resize(windowSize);

  if (!createProgram(g_textureVertexShader, g_textureFragmentShader, &m_textureProgram.program)) {
    return false;
  }

  static F32 vertices[] = {
      0.0f, 0.0f, 0.0f, 0.0f,  //
      1.0f, 0.0f, 1.0f, 0.0f,  //
      1.0f, 1.0f, 1.0f, 1.0f,  //

      1.0f, 1.0f, 1.0f, 1.0f,  //
      0.0f, 1.0f, 0.0f, 1.0f,  //
      0.0f, 0.0f, 0.0f, 0.0f,  //
  };

  if (!createVertexBuffer(vertices, sizeof(vertices), &m_textureProgram.vertexBuffer)) {
    spdlog::warn("Could not create texture program vertex buffer.");
    return false;
  }

  glad_glGenVertexArrays(1, &m_textureProgram.vertexArrayObject);
  if (!clearGlError("Could not create vertex array object.")) {
    return false;
  }

  glad_glBindVertexArray(m_textureProgram.vertexArrayObject);
  if (!clearGlError("Could not bind vertex array object.")) {
    return false;
  }

  glad_glBindBuffer(GL_ARRAY_BUFFER, m_textureProgram.vertexBuffer);
  if (!clearGlError("Could not bind vertex buffer array.")) {
    return false;
  }

  glad_glEnableVertexAttribArray(0);
  if (!clearGlError("Could not enable vertex attribute array.")) {
    return false;
  }

  glad_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(F32), nullptr);
  if (!clearGlError("Could not set vertex attribute pointer. (0)")) {
    return false;
  }

  glad_glEnableVertexAttribArray(1);
  if (!clearGlError("Could not enable vertex attribute array.")) {
    return false;
  }

  glad_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(F32), (void*)(sizeof(F32) * 2));
  if (!clearGlError("Could not set vertex attribute pointer. (1)")) {
    return false;
  }

  glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (!clearGlError("Could not unbind vertex buffer.")) {
    return false;
  }

  glad_glBindVertexArray(0);
  if (!clearGlError("Could not unbind vertex array.")) {
    return false;
  }

  return true;
}

void Renderer::resize(const Size& size) {
  m_windowSize = size;

  GL_CHECK_VOID(glViewport(0, 0, size.width, size.height), "Could not set renderer viewport.");

  spdlog::info("Resize renderer: ({}, {})", size.width, size.height);

  //  m_textureProgram.viewMatrix =
  //      glm::ortho(0.0f, static_cast<F32>(size.width), static_cast<F32>(size.height), 0.0f);
}

void Renderer::setRenderTarget(RenderTargetId renderTarget) {
  m_currentRenderTarget = renderTarget;

  auto* renderTargetData = getData(m_renderTargets, renderTarget);
  GL_CHECK_VOID(glBindFramebuffer(GL_FRAMEBUFFER, renderTargetData->framebuffer),
                "Could not bind frame buffer.");

  //  if (renderTarget != RenderTargetId::invalidValue()) {
  //    m_textureProgram.viewMatrix = glm::ortho(0.0f,
  //    static_cast<F32>(renderTargetData->size.width),
  //                                             static_cast<F32>(renderTargetData->size.height),
  //                                             0.0f);
  //  }
}

void Renderer::clearRenderTarget() {
  m_currentRenderTarget = RenderTargetId::invalidValue();

  GL_CHECK_VOID(glBindFramebuffer(GL_FRAMEBUFFER, 0), "Could not unbind frame buffer.");

  //  m_textureProgram.viewMatrix = glm::ortho(0.0f, static_cast<F32>(m_windowSize.width),
  //                                           static_cast<F32>(m_windowSize.height), 0.0f);
}

TextureId Renderer::createTexture(void* data, const Size& size) {
  auto invalid = TextureId::invalidValue();

  U32 texture;
  GL_CHECK(glGenTextures(1, &texture), "Could not generate textures.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture), "Could not bind texture.", invalid)

  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, data),
           "Could not set texture data.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST),
           "Could not set texture min filter.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST),
           "Could not set texture mag filter.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.", invalid)

  return addData<TextureId>(&m_textures, texture, size);
}

RenderTargetId Renderer::createRenderTarget(const Size& size) {
  auto invalid = RenderTargetId::invalidValue();

  U32 frameBuffer;
  GL_CHECK(glGenFramebuffers(1, &frameBuffer), "Could not create frame buffer.", invalid);
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer), "Could not bind frame buffer.", invalid);

  U32 texture;

  GL_CHECK(glGenTextures(1, &texture), "Could not create color buffer texture.", invalid)
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture), "Could not bind color buffer texture.", invalid)
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, nullptr),
           "Could not allocate color buffer data.", invalid)
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST),
           "Could not set texture min filter.", invalid)
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST),
           "Could not set texture mag filter.", invalid)
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.", invalid)

  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0),
           "Could not attach texture to frame buffer as color attachment.", invalid)

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    spdlog::error("Frame buffer is not complete.");
    return invalid;
  }

  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0), "Could not unbind frame buffer.", invalid);

  return addData<RenderTargetId>(&m_renderTargets, frameBuffer, texture, size);
}

void Renderer::clear(F32 red, F32 green, F32 blue, F32 alpha) {
  GL_CHECK_VOID(glClearColor(red, green, blue, alpha), "Could not set clear color.")
  GL_CHECK_VOID(glClear(GL_COLOR_BUFFER_BIT), "Could not clear.")
}

void Renderer::copyTexture(TextureId texture, const Rect& to) {
  auto textureData = getData(m_textures, texture);
  if (!textureData) {
    spdlog::warn("Rendering texture that does not exist.");
    return;
  }

  Rect from{{0, 0}, textureData->size};
  copyTextureInternal(textureData->texture, from, to);
}

void Renderer::copyTexture(TextureId texture, const Rect& from, const Rect& to) {
  auto textureData = getData(m_textures, texture);
  if (!textureData) {
    spdlog::warn("Rendering texture that does not exist.");
    return;
  }

  copyTextureInternal(textureData->texture, from, to);
}

void Renderer::copyTexture(RenderTargetId renderTarget, const Rect& to) {
  auto renderTargetData = getData(m_renderTargets, renderTarget);
  if (!renderTargetData) {
    spdlog::warn("Rendering texture that does not exist.");
    return;
  }

  Rect from{{0, 0}, renderTargetData->size};
  copyTextureInternal(renderTargetData->texture, from, to);
}

void Renderer::beginFrame() {
  Size windowSize{};
  SDL_GetWindowSize(m_window, &windowSize.width, &windowSize.height);

  // spdlog::info("Window size: ({}, {})", windowSize.width, windowSize.height);

  GL_CHECK_VOID(glViewport(0, 0, windowSize.width, windowSize.height), "Could not set viewport.")
}

void Renderer::finishFrame() {
  SDL_GL_SwapWindow(m_window);
}

void Renderer::copyTextureInternal(U32 texture, const Rect& from, const Rect& to) {
  GL_CHECK_VOID(glUseProgram(m_textureProgram.program), "Could not bind program.")
  GL_CHECK_VOID(glBindVertexArray(m_textureProgram.vertexArrayObject),
                "Could not bind vertex array.")
  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_2D, texture), "Could not bind texture.")

  glm::mat4 viewMatrix;
  if (m_currentRenderTarget != RenderTargetId::invalidValue()) {
    auto* renderTargetData = getData(m_renderTargets, m_currentRenderTarget);
    viewMatrix = glm::ortho(0.0f, static_cast<F32>(renderTargetData->size.width), 0.0f,
                            static_cast<F32>(renderTargetData->size.height));
  } else {
    viewMatrix = glm::ortho(0.0f, static_cast<F32>(m_windowSize.width),
                            static_cast<F32>(m_windowSize.height), 0.0f);
  }

  auto viewMatrixLocation = glad_glGetUniformLocation(m_textureProgram.program, "u_viewMatrix");
  GL_CHECK_VOID(glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix)),
                "Could not set matrix uniform.")

  auto rectLocation = glad_glGetUniformLocation(m_textureProgram.program, "u_rect");
  GL_CHECK_VOID(glUniform4f(rectLocation, static_cast<F32>(to.position.left),
                            static_cast<F32>(to.position.top), static_cast<F32>(to.size.width),
                            static_cast<F32>(to.size.height)),
                "Could not set rect uniform.")

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GL_CHECK_VOID(glDrawArrays(GL_TRIANGLES, 0, 6), "Could not draw arrays.")

  glDisable(GL_BLEND);

  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.")
  GL_CHECK_VOID(glBindVertexArray(0), "Could not unbind vertex array.")
  GL_CHECK_VOID(glUseProgram(0), "Could not unbind program.")
}

}  // namespace renderer
