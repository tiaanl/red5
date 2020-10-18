#include "base/streams/memory_input_stream.h"

namespace base {

MemoryInputStream::MemoryInputStream(const U8* data, MemSize size) : m_data{data}, m_size{size} {}

MemSize MemoryInputStream::getPosition() {
  return m_position;
}

void MemoryInputStream::setPosition(MemSize position) {
  m_position = position;
}

MemSize MemoryInputStream::read(void* out, MemSize size) {
  return 0;
}

}  // namespace base
