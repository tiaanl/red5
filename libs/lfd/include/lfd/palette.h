#pragma once

#include <nucleus/Types.h>

#include <vector>

namespace nu {
class InputStream;
class OutputStream;
}  // namespace nu

class Palette {
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

  void read(nu::InputStream* stream);
  void write(nu::OutputStream* stream);

private:
  U8 m_firstIndex;
  U8 m_lastIndex;
  std::vector<Color> m_colors;
};
