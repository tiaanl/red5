#include "renderer/vertex_buffer_resource.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace renderer {

namespace {

U32 attributeTypeToOpenGlType(AttributeType type) {
  switch (type) {
    case AttributeType::UnsignedInt8:
      return GL_UNSIGNED_BYTE;

    case AttributeType::UnsignedInt16:
      return GL_UNSIGNED_SHORT;

    case AttributeType::UnsignedInt32:
      return GL_UNSIGNED_INT;

    case AttributeType::SignedInt8:
      return GL_BYTE;

    case AttributeType::SignedInt16:
      return GL_SHORT;

    case AttributeType::SignedInt32:
      return GL_INT;

    case AttributeType::Float32:
      return GL_FLOAT;
  }

  assert(false);
  return 0;
}

void destroyVertexArrayInternal(U32 name) {
  // GL_CHECK_VOID(glDeleteVertexArrays(1, &name), "Could not delete vertex array.");
}

}  // namespace

VertexBufferContainer::~VertexBufferContainer() {
  for (auto& data : m_data) {
    destroyVertexArrayInternal(data.id);
  }
}

VertexBufferContainer::Identifier VertexBufferContainer::create(const VertexBufferDefinition& def,
                                                                void* data, MemSize dataSize) {
  auto invalid = Identifier::invalidValue();

  U32 vertexArrayName;
  GL_CHECK(glGenVertexArrays(1, &vertexArrayName), "Could not generate vertex array.", invalid);
  GL_CHECK(glBindVertexArray(vertexArrayName), "Could not bind vertex array.", invalid);

  U32 vertexBufferName;
  GL_CHECK(glGenBuffers(1, &vertexBufferName), "Could not generate array buffer.", invalid);
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName), "Could not bind array buffer.",
           invalid);
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW),
           "Could not set array buffer data.", invalid);

  auto stride = def.stride();

  U32 index = 0;
  U32 offset = 0;
  for (auto& attribute : def) {
    GL_CHECK(glVertexAttribPointer(index, static_cast<U32>(attribute.count),
                                   attributeTypeToOpenGlType(attribute.type), GL_FALSE, stride,
                                   reinterpret_cast<void*>(static_cast<MemSize>(offset))),
             "Could not set vertex attribute pointer.", invalid);
    GL_CHECK(glEnableVertexAttribArray(index), "Could not enable vertex attribute array.", invalid);

    ++index;
    offset += VertexBufferDefinition::sizeOf(attribute);
  }

  GL_CHECK(glBindVertexArray(0), "Could not unbind vertex array.", invalid);
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0), "Could not unbind array buffer.", invalid);

  return emplaceData(vertexArrayName);
}

void VertexBufferContainer::destroy(Identifier id) {
  auto data = getData(id);
  if (data) {
    destroyVertexArrayInternal(data->id);
    data->id = 0;
    removeData(id);
  }
}

}  // namespace renderer
