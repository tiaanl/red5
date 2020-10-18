#pragma once

#include "base/streams/input_stream.h"

namespace base {

class MemoryInputStream : public InputStream {
public:
  MemoryInputStream(const U8* data, MemSize size);

  MemSize getPosition() override;
  void setPosition(MemSize position) override;
  MemSize read(void* out, MemSize size) override;

private:
  const U8* m_data;
  MemSize m_size;
  MemSize m_position = 0;
};

}  // namespace base
