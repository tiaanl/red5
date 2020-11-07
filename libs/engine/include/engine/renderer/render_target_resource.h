#pragma once

#include "resource_container.h"

namespace engine {

struct RenderTargetData {
  U32 framebuffer;
  U32 texture;
  SizeI size;
};

class RenderTargetContainer : public ResourceContainer<RenderTargetData> {
public:
  ~RenderTargetContainer();

  Identifier create(const SizeI& size);
  void destroy(Identifier id);
};

}  // namespace engine
