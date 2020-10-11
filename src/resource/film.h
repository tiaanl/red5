#ifndef RESOURCE_FILM_H_
#define RESOURCE_FILM_H_

#include "lfd.h"

#include <stdint.h>

namespace film {

struct Chunk {
  uint16_t length;
  uint16_t op_code;
};

struct Block {
  char type[4];
  char name[8];
  uint32_t length;
  uint16_t type_index;
  uint16_t number_of_chunks;
  uint16_t chunk_data_length;
  Chunk* chunks;
};

struct Header {
  uint16_t reserved;
  uint16_t number_of_frames;
  uint16_t number_of_blocks;
};

struct FILM {
  Header* header;
  Block* blocks;
};

bool load(lfd::File* file, const char* name, FILM* film);

} // namespace film

#endif // RESOURCE_FILM_H_
