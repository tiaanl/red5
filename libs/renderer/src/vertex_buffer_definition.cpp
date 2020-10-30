#include "renderer/vertex_buffer_definition.h"

namespace renderer {

namespace {

U32 sizeOfType(AttributeType type) {
  switch (type) {
    case AttributeType::UnsignedInt8:
    case AttributeType::SignedInt8:
      return 1;

    case AttributeType::UnsignedInt16:
    case AttributeType::SignedInt16:
      return 2;
    case AttributeType::UnsignedInt32:
    case AttributeType::SignedInt32:
    case AttributeType::Float32:
      return 4;
  }

  assert(false);
  return 0;
}

}  // namespace

// static
U32 VertexBufferDefinition::sizeOf(const Attribute& attribute) {
  U32 size = sizeOfType(attribute.type);

  switch (attribute.count) {
    case ComponentCount::One:
      return size;

    case ComponentCount::Two:
      return size * 2;

    case ComponentCount::Three:
      return size * 3;

    case ComponentCount::Four:
      return size * 4;
  }

  assert(false);
  return 0;
}

void VertexBufferDefinition::addAttribute(AttributeType type, ComponentCount count) {
  m_attributes.emplace_back(Attribute{type, count});
}

U32 VertexBufferDefinition::stride() const {
  U32 size = 0;
  for (auto& attribute : m_attributes) {
    size += sizeOf(attribute);
  }
  return size;
}

}  // namespace renderer
