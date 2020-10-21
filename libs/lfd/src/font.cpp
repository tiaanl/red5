#include "lfd/font.h"

#include <base/streams/input_stream.h>

namespace lfd {

void Font::read(base::InputStream* stream, MemSize size) {
  m_startChar = stream->readU16();
  auto glyphCount = stream->readU16();
  auto bitsPerLine = stream->readU16();
  auto height = stream->readU16();
  m_baseLine = stream->readU16();
  auto reserved = stream->readU16();

  m_glyphs.resize(glyphCount);

  spdlog::info(
      "Font :: startChar: {}, glyphCount: {}, bitsPerLine: {}, height: {}, "
      "baseLine: {}, reserved: {}",
      m_startChar, glyphCount, bitsPerLine, height, m_baseLine, reserved);

  for (U16 i = 0; i < glyphCount; ++i) {
    m_glyphs[i].width = stream->readU8();

    spdlog::info("  Glyph :: width: {}", m_glyphs[i].width);
  }

  assert(bitsPerLine % 8 == 0);
  auto bytesPerLine = bitsPerLine / 8;
  for (U16 i = 0; i < glyphCount; ++i) {
    auto dataSize = bytesPerLine * height;
    m_glyphs[i].data.resize(dataSize);
    stream->read(m_glyphs[i].data.data(), dataSize);
  }
}

void Font::write(base::OutputStream* stream) {}

}  // namespace lfd
