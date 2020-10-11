#include "resource/film.h"

#include <assert.h>

namespace film {

bool load(lfd::File* file, const char* name, FILM* film) {
  Data resource = lfd::get_resource(file, "FILM", name);
  assert(resource.buffer != nullptr);

  uint8_t* p = resource.buffer;

  film->header = (Header*)p;

  film->header = (Header*)resource.buffer;
  film->blocks = (Block*)(resource.buffer + sizeof(Header));

  return true;
}

} // namespace film
