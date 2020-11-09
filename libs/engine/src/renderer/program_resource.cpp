#include "engine/renderer/program_resource.h"

#include <glad/glad.h>

#include "engine/renderer/renderer.h"
#include "gl_check.h"

namespace engine {

namespace {

bool createShaderInternal(U32 type, const char* source, U32* shader) {
  U32 name = glad_glCreateShader(type);
  if (!detail::clearGlError("Could not create vertex shader.", __FILE__, __LINE__)) {
    return false;
  }

  I32 length = static_cast<I32>(std::strlen(source));
  GL_CHECK(glShaderSource(name, 1, &source, &length), "Could not set shader source.", false);

  GL_CHECK(glCompileShader(name), "Could not compile shader program.", false);

  I32 success;
  GL_CHECK(glGetShaderiv(name, GL_COMPILE_STATUS, &success), "Could not get shader success status.",
           false);

  if (!success) {
    char log[1024];
    I32 infoLogLength;
    GL_CHECK(glGetShaderInfoLog(name, 1024, &infoLogLength, log), "Could not get program info log.",
             false);
    log[length] = '\0';
    spdlog::warn("Could not compile shader program.\n{}", log);
    return false;
  }

  *shader = name;

  return true;
}

bool createProgramInternal(const char* vertexShaderSource, const char* fragmentShaderSource,
                           U32* program) {
  U32 vertexShader, fragmentShader;

  if (!createShaderInternal(GL_VERTEX_SHADER, vertexShaderSource, &vertexShader)) {
    return false;
  }

  if (!createShaderInternal(GL_FRAGMENT_SHADER, fragmentShaderSource, &fragmentShader)) {
    return false;
  }

  U32 programName = glad_glCreateProgram();
  if (!detail::clearGlError("Could not create new program.", __FILE__, __LINE__)) {
    return false;
  }

  GL_CHECK(glAttachShader(programName, vertexShader), "Could not attach vertex shader to program.",
           false);

  GL_CHECK(glAttachShader(programName, fragmentShader),
           "Could not attach fragment shader to program.", false);

  GL_CHECK(glLinkProgram(programName), "Could not link program.", false);

  GL_CHECK(glDeleteShader(vertexShader), "Could not delete shader.", false);

  GL_CHECK(glDeleteShader(fragmentShader), "Could not delete shader.", false);

  *program = programName;

  return true;
}

void destroyProgramInternal(U32 name) {
  // GL_CHECK_VOID(glDeleteProgram(name), "Could not delete program.");
}

}  // namespace

ProgramContainer::~ProgramContainer() {
  for (auto& data : m_data) {
    destroyProgramInternal(data.id);
  }
}

ProgramContainer::Identifier ProgramContainer::create(const char* vertexShaderSource,
                                                      const char* fragmentShaderSource) {
  U32 id;
  if (!createProgramInternal(vertexShaderSource, fragmentShaderSource, &id)) {
    return Identifier::invalidValue();
  }

  return emplaceData(id);
}

void ProgramContainer::destroy(Identifier id) {
//  auto data = getData(id);
//  if (data) {
//    destroyProgramInternal(data->id);
//    removeData(id);
//  }
}

}  // namespace engine
