#pragma once

#include "renderer/resource_container.h"

namespace renderer {

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

}  // namespace renderer
