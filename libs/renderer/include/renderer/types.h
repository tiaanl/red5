#pragma once

#include "renderer/program_resource.h"
#include "renderer/render_target_resource.h"
#include "renderer/resource_container.h"
#include "renderer/texture_resource.h"
#include "renderer/vertex_array_resource.h"

namespace renderer {

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

}  // namespace renderer
