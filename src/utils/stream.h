#ifndef UTILS_STREAM_H_
#define UTILS_STREAM_H_

#include "utils/memory.h"

struct InputStream {
  Data data;
  uint8_t* current;
};

InputStream stream_from_data(Data data);

void advance(InputStream* stream, size_t count);

void read(InputStream* stream, char* out, size_t count);

uint8_t read_u8(InputStream* stream);
uint16_t read_u16(InputStream* stream);
uint32_t read_u32(InputStream* stream);

#endif  // UTILS_STREAM_H_
