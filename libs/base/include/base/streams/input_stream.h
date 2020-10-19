#pragma once

namespace base {

class InputStream {
public:
  virtual ~InputStream();

  virtual MemSize getPosition() = 0;
  virtual void setPosition(MemSize position) = 0;
  virtual MemSize read(void* out, MemSize size) = 0;

  void advance(MemSize bytes);

  U8 readU8();
  U16 readU16();
  U32 readU32();
  U64 readU64();

  I8 readI8();
  I16 readI16();
  I32 readI32();
  I64 readI64();

  F32 readF32();
  F64 readF64();
};

}  // namespace base
