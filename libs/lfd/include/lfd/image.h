#pragma once

#include <vector>

#include "base/platform.h"
#include "resource.h"

namespace nu {
class InputStream;
class OutputStream;
}  // namespace nu

class Image : public Resource {
public:
  struct Line {
    U16 left;
    U16 top;
    std::vector<U8> indices;

    Line(U16 left, U16 top, std::vector<U8> indices)
      : left{left}, top{top}, indices{std::move(indices)} {}
  };

  ~Image() override;

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

  U16 width() const {
    return m_right - m_left + 1;
  }

  U16 height() const {
    return m_bottom - m_top + 1;
  }

  const std::vector<Line>& lines() const {
    return m_lines;
  }

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  U16 m_left;
  U16 m_top;
  U16 m_right;
  U16 m_bottom;
  std::vector<Line> m_lines;
};
