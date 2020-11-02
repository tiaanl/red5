#include "renderer/texture_resource.h"

#include <glad/glad.h>

#include "gl_check.h"

namespace renderer {

namespace {

void destroyTextureInternal(U32 name) {
  // GL_CHECK_VOID(glDeleteTextures(1, &name), "Could not delete texture.");
}

}  // namespace

TextureContainer::~TextureContainer() {
  for (auto& data : m_data) {
    if (data.texture != 0) {
      destroyTextureInternal(data.texture);
    }
  }
}

TextureContainer::Identifier TextureContainer::create(const void* data, TextureFormat format,
                                                      const Size& size) {
  auto invalid = Identifier::invalidValue();

  U32 texture;
  GL_CHECK(glGenTextures(1, &texture), "Could not generate textures.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture), "Could not bind texture.", invalid)

  U32 glFormat = 0;
  switch (format) {
    case TextureFormat::RedGreenBlueAlpha:
      glFormat = GL_RGBA;
      break;

    case TextureFormat::RedGreenBlue:
      glFormat = GL_RGB;
      break;

    case TextureFormat::Red:
      glFormat = GL_RED;
      break;
  }
  assert(glFormat != 0);

  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, glFormat, size.width, size.height, 0, glFormat,
                        GL_UNSIGNED_BYTE, data),
           "Could not set texture data.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST),
           "Could not set texture min filter.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST),
           "Could not set texture mag filter.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0), "Could not unbind texture.", invalid)

  return emplaceData(texture, static_cast<U32>(GL_TEXTURE_2D), size);
}

TextureContainer::Identifier TextureContainer::createFromRaw(U32 texture, const Size& size) {
  return emplaceData(texture, static_cast<U32>(GL_TEXTURE_2D), size);
}

TextureContainer::Identifier TextureContainer::create1D(const RGB* data, U32 count) {
  auto invalid = Identifier::invalidValue();

  U32 texture;
  GL_CHECK(glGenTextures(1, &texture), "Could not generate textures.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_1D, texture), "Could not bind texture.", invalid)

  GL_CHECK(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, count, 0, GL_RGB, GL_UNSIGNED_BYTE, data),
           "Could not set texture data.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST),
           "Could not set texture min filter.", invalid)

  GL_CHECK(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST),
           "Could not set texture mag filter.", invalid)

  GL_CHECK(glBindTexture(GL_TEXTURE_1D, 0), "Could not unbind texture.", invalid)

  return emplaceData(texture, static_cast<U32>(GL_TEXTURE_1D), Size{static_cast<I32>(count), 1});
}

void TextureContainer::update1D(Identifier texture, const RGB* data, U32 start, U32 end) {
  auto textureData = getData(texture);
  if (!textureData) {
    spdlog::warn("Texture not found.");
    return;
  }

  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_1D, textureData->texture), "Could not bind texture.")

  GL_CHECK_VOID(
      glTexSubImage1D(GL_TEXTURE_1D, 0, start, end - start + 1, GL_RGB, GL_UNSIGNED_BYTE, data),
      "Could not update texture sub data.")

  GL_CHECK_VOID(glBindTexture(GL_TEXTURE_1D, 0), "Could not unbind texture.")
}
void TextureContainer::destroy(Identifier id) {
  auto data = getData(id);
  if (data) {
    destroyTextureInternal(data->texture);
    removeData(id);
  }
}

}  // namespace renderer
