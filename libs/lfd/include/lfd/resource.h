#pragma once

#include "base/platform.h"

namespace base {
class InputStream;
class OutputStream;
}

class Resource {
public:
  virtual ~Resource() = default;

  virtual void read(base::InputStream* stream, MemSize size) = 0;
  virtual void write(base::OutputStream* stream) = 0;
};
