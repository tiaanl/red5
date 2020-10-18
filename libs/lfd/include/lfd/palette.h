#pragma once

#include "base/platform.h"

#include <vector>

#include "resource.h"

namespace base {
class InputStream;
class OutputStream;
}  // namespace base

class Palette : public Resource {
public:
  struct Color {
    U8 red;
    U8 green;
    U8 blue;
  };

  U8 firstIndex() const {
    return m_firstIndex;
  }

  U8 lastIndex() const {
    return m_lastIndex;
  }

  const std::vector<Color>& colors() const {
    return m_colors;
  }

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  U8 m_firstIndex;
  U8 m_lastIndex;
  std::vector<Color> m_colors;
  std::vector<U32> m_extras;
};
