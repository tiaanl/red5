#pragma once

#include "lfd/resource.h"

namespace base {
class InputStream;
class OutputStream;
}  // namespace base

namespace lfd {

class Craft : public Resource {
public:
  ~Craft() override;

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;
};

}  // namespace lfd
