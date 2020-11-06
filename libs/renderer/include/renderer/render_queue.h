#pragma once

#include "renderer/color.h"
#include "renderer/types.h"
#include "renderer/uniform_data.h"

namespace renderer {

enum class RenderCommandType : U32 {
  ClearRenderTarget,
  CopyRenderTarget,
  RenderVertexBuffer,
  RenderImmediate,
};

struct RenderCommand {
  RenderCommandType type;

  struct ClearRenderTarget {
    RenderTargetId renderTarget;
    ColorF color = {0.0f, 0.0f, 0.0f, 1.0f};
  } clearRenderTarget;

  struct CopyRenderTarget {
    RenderTargetId from;
    RectI fromRect = {0, 0, 0, 0};
    RenderTargetId to;
    RectI toRect = {0, 0, 0, 0};
  } copyRenderTarget;

  struct RenderVertexBuffer {
    RenderTargetId renderTarget;
    VertexArrayId vertexBuffer;
    ProgramId program;
    UniformData uniformData;
  } renderVertexBuffer;

  struct RenderImmediate {
    RenderTargetId renderTarget;
    RenderMode renderMode;
    U32 startIndex;
    U32 count;
  } renderImmediate;

  RenderCommand(RenderCommandType type) : type{type} {}
};

class RenderQueue {
public:
  explicit RenderQueue(const RenderTargetData& windowRenderTarget);

  void clearRenderTarget(RenderTargetId renderTarget, const ColorF& color);
  void copyRenderTarget(RenderTargetId from, const RectI& fromRect, RenderTargetId to,
                        const RectI& toRect);
  void renderVertexBuffer(RenderTargetId renderTarget, VertexArrayId vertexBuffer,
                          ProgramId program, UniformData uniformData);
  void renderImmediate(RenderTargetId renderTarget, RenderMode renderMode, U32 startIndex,
                       U32 count);

  std::vector<RenderCommand> flush();

private:
  const RenderTargetData& m_windowRenderTarget;

  RenderTargetId m_lastRenderTarget;
  std::vector<RenderCommand> m_renderCommands;
};

}  // namespace renderer
