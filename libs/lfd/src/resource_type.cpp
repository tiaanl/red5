#include "lfd/resource_type.h"

#include <cassert>

const char* resourceTypeToString(ResourceType resourceType) {
  switch (resourceType) {
    case ResourceType::Adlib:
      return "Adlib";

    case ResourceType::Animation:
      return "Animation";

    case ResourceType::Bitmap:
      return "Bitmap";

    case ResourceType::Blaster:
      return "Blaster";

    case ResourceType::Craft:
      return "Craft";

    case ResourceType::Film:
      return "Film";

    case ResourceType::Font:
      return "Font";

    case ResourceType::Image:
      return "Image";

    case ResourceType::Midi:
      return "Midi";

    case ResourceType::Palette:
      return "Palette";

    case ResourceType::ResourceMap:
      return "ResourceMap";

    case ResourceType::Roland:
      return "Roland";

    case ResourceType::Table:
      return "Table";

    case ResourceType::Text:
      return "Text";

    case ResourceType::Voice:
      return "Voice";

    default:
      // assert(false);  // Invalid resource type.
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
