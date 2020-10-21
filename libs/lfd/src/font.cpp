#include "lfd/font.h"

#include <base/streams/input_stream.h>

#define TRACE_READ 0

namespace lfd {

void Font::read(base::InputStream* stream, MemSize size) {
  m_startChar = stream->readU16();
  auto glyphCount = stream->readU16();
  m_bitsPerLine = stream->readU16();
  m_height = stream->readU16();
  m_baseLine = stream->readU16();
  auto reserved = stream->readU16();

  m_glyphs.resize(glyphCount);

#if TRACE_READ >= 1
  spdlog::info(
      "Font ({}) :: startChar: {}, glyphCount: {}, bitsPerLine: {}, height: {}, "
      "baseLine: {}, reserved: {}",
      size, m_startChar, glyphCount, m_bitsPerLine, m_height, m_baseLine, reserved);
#endif

  for (U16 i = 0; i < glyphCount; ++i) {
    m_glyphs[i].width = stream->readU8();

#if TRACE_READ >= 1
    spdlog::info("  Glyph :: width: {}", m_glyphs[i].width);
#endif
  }

  assert(m_bitsPerLine % 8 == 0);
  auto bytesPerLine = m_bitsPerLine / 8;

  for (U16 i = 0; i < glyphCount; ++i) {
    m_glyphs[i].data.resize(bytesPerLine * m_height);
    stream->read(m_glyphs[i].data.data(), m_glyphs[i].data.size());
  }
}

void Font::write(base::OutputStream* stream) {}

}  // namespace lfd
