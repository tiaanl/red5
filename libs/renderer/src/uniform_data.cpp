#include "renderer/uniform_data.h"

#include <glm/gtc/type_ptr.hpp>

namespace renderer {

UniformData::UniformData() = default;

void UniformData::set(std::string_view name, F32 value1) {
  auto& uniform = uniformFor(name);
  uniform.type = UniformType::Float1;
  uniform.floatData[0] = value1;
}

void UniformData::set(std::string_view name, F32 value1, F32 value2) {
  auto& uniform = uniformFor(name);
  uniform.type = UniformType::Float2;
  uniform.floatData[0] = value1;
  uniform.floatData[1] = value2;
}

void UniformData::set(std::string_view name, F32 value1, F32 value2, F32 value3) {
  auto& uniform = uniformFor(name);
  uniform.type = UniformType::Float3;
  uniform.floatData[0] = value1;
  uniform.floatData[1] = value2;
  uniform.floatData[2] = value3;
}

void UniformData::set(std::string_view name, F32 value1, F32 value2, F32 value3, F32 value4) {
  auto& uniform = uniformFor(name);
  uniform.type = UniformType::Float4;
  uniform.floatData[0] = value1;
  uniform.floatData[1] = value2;
  uniform.floatData[2] = value3;
  uniform.floatData[3] = value4;
}

void UniformData::set(std::string_view name, const glm::mat4x4& matrix) {
  auto& uniform = uniformFor(name);
  uniform.type = UniformType::Matrix4x4;
  auto ptr = glm::value_ptr(matrix);
  std::copy(ptr, ptr + 16, uniform.floatData);
}

void UniformData::set(std::string_view name, UniformType type, F32* values, MemSize count) {
  auto& uniform = uniformFor(name);
  uniform.type = type;
  std::copy(values, values + count, uniform.floatData);
}

void UniformData::set(std::string_view name, U32 texture) {
  auto& uniform = uniformFor(name);
  uniform.type = UniformType::Texture;
  uniform.unsignedIntData[0] = texture;
}

UniformData::Uniform& UniformData::uniformFor(std::string_view name) {
  auto it = std::find_if(std::begin(m_data), std::end(m_data),
                         [&name](auto& uniform) { return uniform.name == name; });

  if (it == std::end(m_data)) {
    return m_data.emplace_back((name));
  } else {
    return *it;
  }
}

}  // namespace renderer
