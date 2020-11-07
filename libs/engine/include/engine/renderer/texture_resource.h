#pragma once

#include "engine/renderer/color.h"
#include "resource_container.h"

namespace engine {

enum class TextureFormat : U32 {
  RedGreenBlueAlpha,
  RedGreenBlue,
  Red,
};

struct TextureData {
  U32 texture;
  U32 dimensions;  // GL_TEXTURE_1D, GL_TEXTURE_2D, etc.
  SizeI size;
};

class TextureContainer : public ResourceContainer<TextureData> {
public:
  using Identifier = ResourceContainer<TextureData>::Identifier;

  ~TextureContainer();

  Identifier create(const void* data, TextureFormat format, const SizeI& size);
  Identifier createFromRaw(U32 texture, const SizeI& size);

  Identifier create1D(const RGB* data, U32 count);
  void update1D(Identifier texture, const RGB* data, U32 start, U32 end);

  void destroy(Identifier id);
};

}  // namespace engine
