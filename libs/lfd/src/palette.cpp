#include "lfd/palette.h"

#include <base/streams/input_stream.h>
#include <base/streams/output_stream.h>

#define TRACE_READ 0

void Palette::read(base::InputStream* stream, MemSize size) {
  m_firstIndex = stream->readU8();
  m_lastIndex = stream->readU8();

  U8 colorCount = m_lastIndex - m_firstIndex + 1;

#if TRACE_READ
  spdlog::info("Palette ({}) :: firstIndex: {}, lastIndex: {}, colorCount: {}", size, m_firstIndex,
               m_lastIndex, colorCount);
#endif

  m_colors.resize(colorCount);
  stream->read(m_colors.data(), m_colors.size() * sizeof(Color));

  // Read extra data.
  auto extraCount = stream->readU8();
  for (U8 i = 0; i < extraCount; ++i) {
    m_extras.emplace_back(stream->readU32());
  }
}

void Palette::write(base::OutputStream* stream) {
  stream->writeU8(m_firstIndex);
  stream->writeU8(m_lastIndex);

  stream->write(m_colors.data(), m_colors.size() * sizeof(Color));

  stream->writeU8(static_cast<U8>(m_extras.size()));
  for (auto extra : m_extras) {
    stream->writeU32(extra);
  }
}
