#ifndef UTILS_MEMORY_H_
#define UTILS_MEMORY_H_

#include <stdint.h>

struct Data {
  uint8_t* buffer;
  size_t size;
};

struct Allocator {
  Data data;
  uint8_t* current;
};

void new_allocator(Allocator* allocator, size_t size);
void free_allocator(Allocator* allocator);

Data allocate(Allocator* allocator, size_t size);
void deallocate(Allocator* allocator, Data* data);

#endif // UTILS_MEMORY_H_
