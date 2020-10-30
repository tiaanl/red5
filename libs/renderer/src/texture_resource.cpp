#include "renderer/texture_resource.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace renderer {

namespace {

void destroyTextureInternal(U32 name) {
  GL_CHECK_VOID(glDeleteTextures(1, &name), "Could not delete texture.");
}

}  // namespace

TextureContainer::~TextureContainer() {
  for (auto& data : m_data) {
    if (data.texture != 0) {
      destroyTextureInternal(data.texture);
    }
  }
}

TextureContainer::Identifier TextureContainer::create(const void* data, const Size& size) {
  auto invalid = Identifier::invalidValue();

  U32 texture;
  GL_CHECK(glGenTextures(1, &texture), "Could not generate textures.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture), "Could not bind texture.", invalid)

  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, data),
           "Could not set texture data.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST),
           "Could not set texture min filter.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST),
           "Could not set texture mag filter.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.", invalid)

  return emplaceData(texture, size);
}

void TextureContainer::destroy(Identifier id) {
  auto data = getData(id);
  if (data) {
    destroyTextureInternal(data->texture);
    removeData(id);
  }
}

}  // namespace renderer
