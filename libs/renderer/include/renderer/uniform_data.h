#pragma once

#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

#include "renderer/types.h"

namespace renderer {

enum class UniformType : U32 {
  Unknown,
  Float1,
  Float2,
  Float3,
  Float4,
  Matrix4X4,
  Texture,
};

class UniformData {
public:
  UniformData();

  void set(std::string_view name, F32 value1);
  void set(std::string_view name, F32 value1, F32 value2);
  void set(std::string_view name, F32 value1, F32 value2, F32 value3);
  void set(std::string_view name, F32 value1, F32 value2, F32 value3, F32 value4);
  void set(std::string_view name, const glm::mat4x4& matrix);
  void set(std::string_view name, UniformType type, F32* values, MemSize count);
  void set(std::string_view name, TextureId texture);

private:
  friend class Renderer;

  struct Uniform {
    std::string name;
    UniformType type;
    union {
      F32 floatData[16];
      U32 unsignedIntData[16];
      TextureId texture;
    };

    explicit Uniform(std::string_view name) : name{name}, type{UniformType::Unknown}, floatData{} {}
  };

  Uniform& uniformFor(std::string_view name);

  std::vector<Uniform> m_data;
};

}  // namespace renderer
