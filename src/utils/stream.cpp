#include "utils/stream.h"

#include <string.h>

InputStream stream_from_data(Data data) {
  return {data, data.buffer};
}

void advance(InputStream* stream, size_t count) {
  stream->current += count;
}

void read(InputStream* stream, char* out, size_t count) {
  memcpy(out, stream->current, count);
  stream->current += count;
}

uint8_t read_u8(InputStream* stream) {
  return *stream->current++;
}

uint16_t read_u16(InputStream* stream) {
  uint16_t v = *(uint16_t*)stream->current;
  stream->current += sizeof(uint16_t);
  return v;
}

uint32_t read_u32(InputStream* stream) {
  uint32_t v = *(uint32_t*)stream->current;
  stream->current += sizeof(uint32_t);
  return v;
}
