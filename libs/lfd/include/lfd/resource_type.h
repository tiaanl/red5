#pragma once

#include <nucleus/Types.h>

#include <string>

enum class ResourceType : U32 {
  Animation = 0x4D494E41,
  Film = 0x4d4c4946,
  Image = 0x544C4544,
  Palette = 0x54544C50,
  ResourceMap = 0x50414d52,
  Voice = 0x43494F56,

  Unknown = 0,
};

const char* resourceTypeToString(ResourceType resourceType);
ResourceType resourceTypeFromU32(U32 value);
