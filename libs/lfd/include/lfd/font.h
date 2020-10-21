#pragma once

#include "lfd/resource.h"

namespace base {
class InputStream;
class OutputStream;
}  // namespace base

namespace lfd {

class Font : public Resource {
public:
  struct Glyph {
    U8 width;
    std::vector<U8> data;
  };

  U16 startChar() const {
    return m_startChar;
  }

  U16 bitsPerLine() const {
    return m_bitsPerLine;
  }

  U16 height() const {
    return m_height;
  }

  U16 baseLine() const {
    return m_baseLine;
  }

  const std::vector<Glyph>& glyphs() const {
    return m_glyphs;
  }

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  U16 m_startChar = 0;
  U16 m_bitsPerLine = 0;
  U16 m_height = 0;
  U16 m_baseLine = 0;
  std::vector<Glyph> m_glyphs;
};

}  // namespace lfd
