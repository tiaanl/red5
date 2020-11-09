#pragma once

#include <string_view>

namespace engine::detail {

bool clearGlError(std::string_view failureMessage, const char* file, int line);

}  // namespace engine::detail

#define GL_CHECK(Code, FailureMessage, FailReturn)                                                 \
  {                                                                                                \
    glad_##Code;                                                                                   \
    if (!::engine::detail::clearGlError(FailureMessage, __FILE__, __LINE__)) {                     \
      return (FailReturn);                                                                         \
    }                                                                                              \
  }

#define GL_CHECK_VOID(Code, FailureMessage)                                                        \
  {                                                                                                \
    glad_##Code;                                                                                   \
    if (!::engine::detail::clearGlError(FailureMessage, __FILE__, __LINE__)) {                     \
      return;                                                                                      \
    }                                                                                              \
  }
