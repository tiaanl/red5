#pragma once

#include <array>

namespace renderer {

enum class AttributeType : U8 {
  UnsignedInt8,
  UnsignedInt16,
  UnsignedInt32,
  SignedInt8,
  SignedInt16,
  SignedInt32,
  Float32,
};

enum class ComponentCount : U8 {
  One = 0x01,
  Two = 0x02,
  Three = 0x03,
  Four = 0x04,
};

class VertexBufferDefinition {
public:
  struct Attribute {
    AttributeType type;
    ComponentCount count;
  };

  using AttributeList = std::vector<Attribute>;

  static U32 sizeOf(const Attribute& attribute);

  void addAttribute(AttributeType type, ComponentCount count);

  U32 stride() const;

  AttributeList::const_iterator begin() const {
    return std::cbegin(m_attributes);
  }

  AttributeList::const_iterator end() const {
    return std::cend(m_attributes);
  }

private:
  std::vector<Attribute> m_attributes;
};

}  // namespace renderer
