#include "utils/memory.h"

#include <assert.h>
#include <stdlib.h>

void new_allocator(Allocator* allocator, size_t size) {
  allocator->data.buffer = (uint8_t*)malloc(size);
  allocator->data.size = size;
  allocator->current = allocator->data.buffer;
}

void free_allocator(Allocator* allocator) {
  free(allocator->data.buffer);
  allocator->data.buffer = nullptr;
  allocator->data.size = 0;
  allocator->current = nullptr;
}

Data allocate(Allocator* allocator, size_t size) {
  assert(allocator->current + size < allocator->data.buffer + allocator->data.size);

  Data result{allocator->current, size};

  allocator->current += size;

  return result;
}

void deallocate(Allocator* allocator, Data* data) {
  // noop
}
