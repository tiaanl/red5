#include "lfd/palette.h"

#include "base/streams/input_stream.h"
#include "base/streams/output_stream.h"

#define TRACE_LOAD 0

void Palette::read(base::InputStream* stream, MemSize size) {
  m_firstIndex = stream->readU8();
  m_lastIndex = stream->readU8();

#if TRACE_LOAD
  LOG(Info) << "Palette :: firstIndex: " << static_cast<U32>(m_firstIndex)
            << ", lastIndex: " << static_cast<U32>(m_lastIndex);
#endif

  m_colors.resize(m_lastIndex - m_firstIndex + 1);
  stream->read(m_colors.data(), m_colors.size() * sizeof(Color));
}

void Palette::write(base::OutputStream* stream) {
  stream->writeU8(m_firstIndex);
  stream->writeU8(m_lastIndex);
  stream->write(m_colors.data(), m_colors.size() * sizeof(Color));
  // TODO: This might not be read as 0 and then there is more data required.
  stream->writeU8(0);
}
