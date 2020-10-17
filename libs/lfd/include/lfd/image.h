#pragma once

#include <nucleus/Types.h>

#include <vector>

namespace nu {
class InputStream;
class OutputStream;
}  // namespace nu

class Image {
public:
  struct Line {
    U16 left;
    U16 top;
    std::vector<U8> indices;
  };

  U16 left() const {
    return m_left;
  }

  U16 top() const {
    return m_top;
  }

  U16 right() const {
    return m_right;
  }

  U16 bottom() const {
    return m_bottom;
  }

  const std::vector<Line>& lines() const {
    return m_lines;
  }

  void read(nu::InputStream* stream);
  void write(nu::OutputStream* stream);

private:
  U16 m_left;
  U16 m_top;
  U16 m_right;
  U16 m_bottom;
  std::vector<Line> m_lines;
};
