
#include "base/streams/file_output_stream.h"

namespace base {

FileOutputStream::FileOutputStream(std::filesystem::path path) : m_path{std::move(path)} {}

FileOutputStream::~FileOutputStream() = default;

MemSize FileOutputStream::getPosition() const {
  return 0;
}

void FileOutputStream::setPosition(MemSize position) {}

MemSize FileOutputStream::write(const void* data, MemSize size) {
  return 0;
}

}  // namespace base
