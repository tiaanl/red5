#pragma once

#include "renderer/dimensions.h"
#include "renderer/resource_container.h"

namespace renderer {

struct RenderTargetData {
  U32 framebuffer;
  U32 texture;
  Size size;
};

class RenderTargetContainer : public ResourceContainer<RenderTargetData> {
public:
  ~RenderTargetContainer();

  Identifier create(const Size& size);
  void destroy(Identifier id);
};

}  // namespace renderer
