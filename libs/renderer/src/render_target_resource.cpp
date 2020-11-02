#include "renderer/render_target_resource.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace renderer {

namespace {

void destroyRenderTargetInternal(RenderTargetData& data) {
#if 0
  GL_CHECK_VOID(glDeleteTextures(1, &data.texture), "Could not delete renderInternal target texture.");
  GL_CHECK_VOID(glDeleteBuffers(1, &data.framebuffer),
                "Could not delete renderInternal target frame buffer.");
#endif  // 0
}

}  // namespace

RenderTargetContainer::~RenderTargetContainer() {
  for (auto& data : m_data) {
    destroyRenderTargetInternal(data);
  }
}

RenderTargetContainer::Identifier RenderTargetContainer::create(const Size& size) {
  auto invalid = Identifier::invalidValue();

  U32 frameBuffer;
  GL_CHECK(glGenFramebuffers(1, &frameBuffer), "Could not create frame buffer.", invalid);
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer), "Could not bind frame buffer.", invalid);

  U32 texture;

  GL_CHECK(glGenTextures(1, &texture), "Could not create color buffer texture.", invalid)
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture), "Could not bind color buffer texture.", invalid)
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, nullptr),
           "Could not allocate color buffer data.", invalid)
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST),
           "Could not set texture min filter.", invalid)
  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST),
           "Could not set texture mag filter.", invalid)
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.", invalid)

  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0),
           "Could not attach texture to frame buffer as color attachment.", invalid)

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    spdlog::error("Frame buffer is not complete.");
    return invalid;
  }

  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0), "Could not unbind frame buffer.", invalid);

  return emplaceData(frameBuffer, texture, size);
}

void RenderTargetContainer::destroy(Identifier id) {
  auto data = getData(id);
  if (data) {
    destroyRenderTargetInternal(*data);
    removeData(id);
  }
}

}  // namespace renderer