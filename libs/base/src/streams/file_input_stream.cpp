#include "base/streams/file_input_stream.h"

namespace base {

FileInputStream::FileInputStream(std::filesystem::path path) : m_stream{path, std::ios::binary} {}

FileInputStream::~FileInputStream() = default;

MemSize FileInputStream::getPosition() {
  return m_stream.tellg();
}

void FileInputStream::setPosition(MemSize position) {
  m_stream.seekg(position);
}

MemSize FileInputStream::read(void* out, MemSize size) {
  auto pos = m_stream.tellg();
  m_stream.read(reinterpret_cast<char*>(out), size);
  return m_stream.tellg() - pos;
}

}  // namespace base
