#pragma once

#include "renderer/resource_container.h"
#include "renderer/vertex_buffer_definition.h"

namespace renderer {

struct VertexBufferData {
  U32 id;
};

class VertexBufferContainer : public ResourceContainer<VertexBufferData> {
public:
  ~VertexBufferContainer();

  Identifier create(const VertexBufferDefinition& def, void* data, MemSize dataSize);
  void destroy(Identifier id);
};

}  // namespace
