#include "base/streams/output_stream.h"

namespace base {

OutputStream::~OutputStream() = default;

#define DEFINE_WRITE(Type)                                                                         \
  void OutputStream::write##Type(Type value) {                                                     \
    write(reinterpret_cast<const U8*>(&value), sizeof(value));                                     \
  }

DEFINE_WRITE(U8)
DEFINE_WRITE(U16)
DEFINE_WRITE(U32)
DEFINE_WRITE(U64)
DEFINE_WRITE(I8)
DEFINE_WRITE(I16)
DEFINE_WRITE(I32)
DEFINE_WRITE(I64)
DEFINE_WRITE(F32)
DEFINE_WRITE(F64)

}  // namespace base
