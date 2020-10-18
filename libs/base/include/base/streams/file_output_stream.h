#pragma once

#include <filesystem>

#include "base/streams/output_stream.h"

namespace base {

class FileOutputStream : public OutputStream {
public:
  explicit FileOutputStream(std::filesystem::path path);
  ~FileOutputStream() override;

  MemSize getPosition() const override;
  void setPosition(MemSize position) override;
  MemSize write(const void* data, MemSize size) override;

private:
  std::filesystem::path m_path;
};

}  // namespace base
