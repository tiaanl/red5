#pragma once

#include "renderer/dimensions.h"
#include "renderer/resource_container.h"

namespace renderer {

struct TextureData {
  U32 texture;
  Size size;
};

class TextureContainer : public ResourceContainer<TextureData> {
public:
  using Identifier = ResourceContainer<TextureData>::Identifier;

  ~TextureContainer();

  Identifier create(const void* data, const Size& size);
  void destroy(Identifier id);
};

}  // namespace renderer
