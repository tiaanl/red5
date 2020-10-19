#pragma once

namespace base {
class InputStream;
class OutputStream;
}  // namespace base

class Resource {
public:
  virtual ~Resource() = default;

  virtual void read(base::InputStream* stream, MemSize size) = 0;
  virtual void write(base::OutputStream* stream) = 0;
};
