#pragma once

#include <filesystem>
#include <fstream>

#include "base/streams/output_stream.h"

namespace base {

class FileOutputStream : public OutputStream {
public:
  explicit FileOutputStream(std::filesystem::path path);
  ~FileOutputStream() override;

  void write(const void* data, MemSize size) override;

private:
  std::ofstream m_stream;
};

}  // namespace base
