#include "lfd/palette.h"

#include <nucleus/Streams/InputStream.h>
#include <nucleus/Streams/OutputStream.h>

void Palette::load(nu::InputStream* stream) {
  m_firstIndex = stream->readU8();
  m_lastIndex = stream->readU8();

  m_colors.resize(m_lastIndex - m_firstIndex);
  stream->read(m_colors.data(), m_colors.size());
}

void Palette::write(nu::OutputStream* stream) {
  stream->writeU8(m_firstIndex);
  stream->writeU8(m_lastIndex);
  stream->write(m_colors.data(), m_colors.size());
}
