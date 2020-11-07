#pragma once

#include "program_resource.h"
#include "render_target_resource.h"
#include "resource_container.h"
#include "texture_resource.h"
#include "vertex_array_resource.h"

namespace engine {

using TextureId = TextureContainer::Identifier;
using VertexArrayId = VertexArrayContainer::Identifier;
using ProgramId = ProgramContainer::Identifier;
using RenderTargetId = RenderTargetContainer::Identifier;

struct ImmediateVertex {
  PositionF position;
  ColorF color;
};

enum class RenderMode : U32 {
  Lines,
  LineStrip,
  LineLoop,
  Triangles,
  TriangleStrip,
};

}  // namespace engine
