#pragma once

#include <string_view>

namespace renderer::detail {

bool clearGlError(std::string_view failureMessage, const char* file, int line);

}  // namespace renderer::detail

#define GL_CHECK(Code, FailureMessage, FailReturn)                                                 \
  {                                                                                                \
    glad_##Code;                                                                                   \
    if (!::renderer::detail::clearGlError(FailureMessage, __FILE__, __LINE__)) {                   \
      return (FailReturn);                                                                         \
    }                                                                                              \
  }

#define GL_CHECK_VOID(Code, FailureMessage)                                                        \
  {                                                                                                \
    glad_##Code;                                                                                   \
    if (!::renderer::detail::clearGlError(FailureMessage, __FILE__, __LINE__)) {                   \
      return;                                                                                      \
    }                                                                                              \
  }
