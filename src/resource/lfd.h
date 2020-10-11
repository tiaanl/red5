#ifndef RESOURCE_LFD_H_
#define RESOURCE_LFD_H_

#include "utils/memory.h"

#include <stdint.h>

namespace lfd {

struct File {
  Allocator* allocator;
  Data data;
};

struct Header {
  char type[4];
  char name[8];
  uint32_t size;
};

bool load_file(const char* path, Allocator* allocator, File* file);
void free_file(File* file);

Header* get_header(File* file);
Data get_resource(File* file, const char* type, const char* name);

#if !defined(NDEBUG)

void print_header(Header* header);
void print_headers(File* file);

#endif

} // namespace lfd

#endif // RESOURCE_LFD_H_
