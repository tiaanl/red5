#pragma once

#include <filesystem>
#include <fstream>

#include "base/streams/input_stream.h"

namespace base {

class FileInputStream : public InputStream {
public:
  explicit FileInputStream(std::filesystem::path path);
  ~FileInputStream() override;

  MemSize getPosition() override;
  void setPosition(MemSize position) override;
  MemSize read(void* out, MemSize size) override;

private:
  std::ifstream m_stream;
};

}  // namespace base
