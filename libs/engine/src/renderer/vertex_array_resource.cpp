#include "engine/renderer/vertex_array_resource.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace engine {

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

VertexArrayContainer::~VertexArrayContainer() {
  for (auto& data : m_data) {
    destroyVertexArrayInternal(data.vertexArrayId);
  }
}

VertexArrayContainer::Identifier VertexArrayContainer::create(const VertexBufferDefinition& def,
                                                              void* data, MemSize dataSize) {
  auto invalid = Identifier::invalidValue();

  U32 vertexArrayId;
  GL_CHECK(glGenVertexArrays(1, &vertexArrayId), "Could not generate vertex array.", invalid);
  GL_CHECK(glBindVertexArray(vertexArrayId), "Could not bind vertex array.", invalid);

  U32 arrayBufferId;
  GL_CHECK(glGenBuffers(1, &arrayBufferId), "Could not generate array buffer.", invalid);
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, arrayBufferId), "Could not bind array buffer.", invalid);
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

  return emplaceData(vertexArrayId, arrayBufferId);
}

void VertexArrayContainer::replace(Identifier id, const void* data, MemSize dataSize) {
  auto* vertexArrayData = getData(id);
  if (!vertexArrayData) {
    spdlog::error("Vertex buffer not found.");
    return;
  }

  GL_CHECK_VOID(glBindBuffer(GL_ARRAY_BUFFER, vertexArrayData->arrayBufferId),
                "Could not bind vertex buffer.");
  GL_CHECK_VOID(glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW),
                "Could not replace array buffer sub data.");
  GL_CHECK_VOID(glBindBuffer(GL_ARRAY_BUFFER, 0), "Could not unbind vertex buffer.");
}

void VertexArrayContainer::destroy(Identifier id) {
//  auto data = getData(id);
//  if (data) {
//    destroyVertexArrayInternal(data->vertexArrayId);
//    data->vertexArrayId = 0;
//    removeData(id);
//  }
}

}  // namespace engine
