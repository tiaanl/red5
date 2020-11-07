#pragma once

#include "resource_container.h"
#include "vertex_buffer_definition.h"

namespace engine {

struct VertexArrayData {
  U32 vertexArrayId;
  U32 arrayBufferId;
};

class VertexArrayContainer : public ResourceContainer<VertexArrayData> {
public:
  ~VertexArrayContainer();

  Identifier create(const VertexBufferDefinition& def, void* data, MemSize dataSize);
  void replace(Identifier id, const void* data, MemSize dataSize);
  void destroy(Identifier id);
};

}  // namespace
