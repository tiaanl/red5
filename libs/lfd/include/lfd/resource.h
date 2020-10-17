#pragma once

#include <nucleus/Types.h>

namespace nu {
class InputStream;
class OutputStream;
}  //

class Resource {
public:
  virtual ~Resource() = default;

  virtual void read(nu::InputStream* stream, MemSize size) = 0;
  virtual void write(nu::OutputStream* stream) = 0;
};
