
#include "resource/lfd.h"

#include <assert.h>
#include <cstring>
#include <stdio.h>

namespace lfd {

namespace {

bool equals(Header* header, const char* type, const char* name) {
  char name_buf[8] = {};
  memcpy(name_buf, name, strlen(name));

  auto result_1 = memcmp(header->type, type, 4) == 0;
  auto result_2 = memcmp(header->name, name_buf, 8) == 0;

  return result_1 && result_2;
}

} // namespace

bool load_file(const char* path, Allocator* allocator, File* file) {
  assert(file->allocator == nullptr);

  FILE* handle = fopen(path, "rb");

  fseek(handle, 0, SEEK_END);
  size_t file_size = ftell(handle);
  fseek(handle, SEEK_SET, 0);

  file->allocator = allocator;
  file->data = allocate(allocator, file_size);

  fread(file->data.buffer, 1, file_size, handle);

  fclose(handle);

  return true;
}

void free_file(File* file) {
  deallocate(file->allocator, &file->data);
  file->allocator = nullptr;
  file->data.buffer = nullptr;
  file->data.size = 0;
}

Header* get_header(File* file) {
  return (Header*)file->data.buffer;
}

Data get_resource(File* file, const char* type, const char* name) {
  Header* header = get_header(file);
  size_t entry_count = header->size / sizeof(Header);

  size_t offset = sizeof(Header) + (entry_count * sizeof(Header));

  Header* current = &header[1];
  for (size_t i = 0; i < entry_count; ++i) {
    if (equals(current, type, name)) {
      break;
    }

    offset += current->size + sizeof(Header);
    ++current;
  }

  return {file->data.buffer + offset + sizeof(Header), current->size};
}

#if !defined(NDEBUG)

void print_header(Header* header) {
  char buf[128];

  memcpy(buf, header->type, sizeof(header->type));
  buf[4] = ' ';
  memcpy(buf + 5, header->name, sizeof(header->name));
  buf[13] = '\0';

  printf("%s %d\n", buf, header->size);
}

void print_headers(File* file) {
  Header* header = get_header(file);
  size_t resource_count = header->size / sizeof(Header);
  ++header;

  for (size_t i = 0; i < resource_count; ++i) {
    print_header(header++);
  }
}

#endif

} // namespace lfd
