#include <glad/glad.h>

namespace renderer::detail {

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

bool clearGlError(std::string_view failureMessage, const char* file, int line) {
#if 0
  return true;
#else
  auto error = glad_glGetError();
  if (error == GL_NO_ERROR) {
    return true;
  }

  spdlog::error("{} ({}) {}:{}", failureMessage, openGlErrorToString(error), file, line);

  return false;
#endif
}

}  // namespace renderer::detail
