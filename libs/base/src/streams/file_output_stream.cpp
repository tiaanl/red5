
#include "base/streams/file_output_stream.h"

namespace base {

FileOutputStream::FileOutputStream(std::filesystem::path path) : m_stream{path} {}

FileOutputStream::~FileOutputStream() = default;

void FileOutputStream::write(const void* data, MemSize size) {
  m_stream.write(static_cast<const char*>(data), size);
}

}  // namespace base
