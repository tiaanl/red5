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

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  U16 m_startChar = 0;
  U16 m_baseLine = 0;
  std::vector<Glyph> m_glyphs;
};

}  // namespace lfd
