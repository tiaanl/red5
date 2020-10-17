#pragma once

#include <nucleus/Types.h>

#include <string>

enum class ResourceType : U32 {
  Adlib = 0x424c4441,
  Animation = 0x4D494E41,
  Bitmap = 0x504d5442,
  Blaster = 0x53414c42,
  Craft = 0x54465243,
  Film = 0x4d4c4946,
  Font = 0x544e4f46,
  Image = 0x544C4544,
  Midi = 0x44494d47,
  Palette = 0x54544C50,
  ResourceMap = 0x50414d52,
  Roland = 0x444e4c52,
  Table = 0x4c424154,
  Text = 0x54584554,
  Voice = 0x43494F56,
};

const char* resourceTypeToString(ResourceType resourceType);
ResourceType resourceTypeFromU32(U32 value);
