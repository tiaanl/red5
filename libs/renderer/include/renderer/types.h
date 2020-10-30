#pragma once

#include "renderer/program_resource.h"
#include "renderer/render_target_resource.h"
#include "renderer/resource_container.h"
#include "renderer/texture_resource.h"
#include "renderer/vertex_buffer_resource.h"

namespace renderer {

using TextureId = TextureContainer::Identifier;
using VertexBufferId = VertexBufferContainer::Identifier;
using ProgramId = ProgramContainer::Identifier;
using RenderTargetId = RenderTargetContainer::Identifier;

}  // namespace renderer
