
#include "base/streams/input_stream.h"

namespace base {

InputStream::~InputStream() = default;

void InputStream::advance(MemSize bytes) {
  setPosition(getPosition() + bytes);
}

#define READ_TYPE(Type)                                                                            \
  Type InputStream::read##Type() {                                                                 \
    Type value;                                                                                    \
    read(&value, sizeof(value));                                                                   \
    return value;                                                                                  \
  }

READ_TYPE(U8)
READ_TYPE(U16)
READ_TYPE(U32)
READ_TYPE(U64)
READ_TYPE(I8)
READ_TYPE(I16)
READ_TYPE(I32)
READ_TYPE(I64)
READ_TYPE(F32)
READ_TYPE(F64)

}  // namespace base
