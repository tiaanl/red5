#pragma once

namespace base {

class OutputStream {
public:
  virtual ~OutputStream();

  virtual void write(const void* data, MemSize size) = 0;

  void writeU8(U8 value);
  void writeU16(U16 value);
  void writeU32(U32 value);
  void writeU64(U64 value);

  void writeI8(I8 value);
  void writeI16(I16 value);
  void writeI32(I32 value);
  void writeI64(I64 value);

  void writeF32(F32 value);
  void writeF64(F64 value);
};

}  // namespace base
