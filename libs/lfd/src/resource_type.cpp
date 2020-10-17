#include "lfd/resource_type.h"

#include <cassert>

const char* resourceTypeToString(ResourceType resourceType) {
  switch (resourceType) {
    case ResourceType::Animation:
      return "Animation";

    case ResourceType::Film:
      return "Film";

    case ResourceType::Image:
      return "Image";

    case ResourceType::Palette:
      return "Palette";

    case ResourceType::ResourceMap:
      return "ResourceMap";

    case ResourceType::Voice:
      return "Voice";

    default:
      return nullptr;
  }
}

ResourceType resourceTypeFromU32(U32 value) {
#if !defined(NDEBUG)
#define EQ(Type) (value == static_cast<U32>(ResourceType::Type))
  assert((EQ(Animation) || EQ(Film) || EQ(Image) || EQ(Palette) || EQ(ResourceMap) || EQ(Voice)));
#undef EQ
#endif

  return static_cast<ResourceType>(value);
}
