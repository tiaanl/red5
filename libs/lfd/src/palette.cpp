#include "lfd/palette.h"

#include <nucleus/Streams/InputStream.h>
#include <nucleus/Streams/OutputStream.h>

#define TRACE_LOAD 0

#if TRACE_LOAD
#include <nucleus/Logging.h>
#endif

void Palette::read(nu::InputStream* stream) {
  m_firstIndex = stream->readU8();
  m_lastIndex = stream->readU8();

#if TRACE_LOAD
  LOG(Info) << "Palette :: firstIndex: " << static_cast<U32>(m_firstIndex)
            << ", lastIndex: " << static_cast<U32>(m_lastIndex);
#endif

  m_colors.resize(m_lastIndex - m_firstIndex + 1);
  stream->read(m_colors.data(), m_colors.size() * sizeof(Color));
}

void Palette::write(nu::OutputStream* stream) {
  stream->writeU8(m_firstIndex);
  stream->writeU8(m_lastIndex);
  stream->write(m_colors.data(), m_colors.size() * sizeof(Color));
  // TODO: This might not be read as 0 and then there is more data required.
  stream->writeU8(0);
}
