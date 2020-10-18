#include "base/streams/memory_input_stream.h"

namespace base {

MemoryInputStream::MemoryInputStream(const U8* data, MemSize size) : m_data{data}, m_size{size} {}

MemoryInputStream::MemoryInputStream(const std::vector<U8>& data)
  : m_data{data.data()}, m_size{data.size()} {}

MemSize MemoryInputStream::getPosition() {
  return m_position;
}

void MemoryInputStream::setPosition(MemSize position) {
  m_position = position;
}

MemSize MemoryInputStream::read(void* out, MemSize size) {
  MemSize bytesToRead = std::min(size, m_size - m_position);
  std::memcpy(out, &m_data[m_position], bytesToRead);
  m_position += bytesToRead;
  return bytesToRead;
}

}  // namespace base
