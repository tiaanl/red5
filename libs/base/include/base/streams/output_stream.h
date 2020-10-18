#pragma once

#include "base/platform.h"

namespace base {

class OutputStream {
public:
  virtual ~OutputStream();

  virtual MemSize getPosition() const = 0;
  virtual void setPosition(MemSize position) = 0;
  virtual MemSize write(const void* data, MemSize size) = 0;

  MemSize writeU8(U8 value);
  MemSize writeU16(U16 value);
  MemSize writeU32(U32 value);
  MemSize writeU64(U64 value);

  MemSize writeI8(I8 value);
  MemSize writeI16(I16 value);
  MemSize writeI32(I32 value);
  MemSize writeI64(I64 value);

  MemSize writeF32(F32 value);
  MemSize writeF64(F64 value);
};

}  // namespace base
