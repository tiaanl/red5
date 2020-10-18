#include "props.h"

#include <base/logging.h>

namespace {

void drawImage(const Image& image, const RenderState& renderState, const Prop::Offset& offset) {
  // LOG(Info) << "Drawing image at (" << image.left() << ", " << image.top() << ")";
  for (auto& line : image.lines()) {
    if (line.top + offset.y >= renderState.screenHeight) {
      break;
    }
    MemSize pos = (line.top + offset.y) * renderState.screenWidth + (line.left + offset.x);
    for (auto index : line.indices) {
      if (line.left + offset.x + index >= renderState.screenWidth) {
        break;
      }
      renderState.pixels[pos++] = renderState.palette[index];
    }
  }
}

}  // namespace

Prop::Prop(std::vector<Film::Chunk> chunks) : m_chunks{std::move(chunks)} {}

void Prop::updateState(U32 frame) {
  U32 chunkTime = 0;

  for (auto& chunk : m_chunks) {
    switch (chunk.opCode) {
      case OpCode::Time: {
        chunkTime = chunk.variables[0];
        break;
      }

      case OpCode::Layer: {
        if (frame >= chunkTime) {
          m_layer = chunk.variables[0];
        }
        break;
      }

      case OpCode::Display: {
        if (frame >= chunkTime) {
          m_visible = chunk.variables[0] != 0;
        }
        break;
      }

      case OpCode::Move: {
        m_movePerFrame.x = chunk.variables[0];
        m_movePerFrame.y = chunk.variables[1];
        break;
      }

      case OpCode::End: {
        break;
      }

      default: {
        // lg::info("OpCode not handled: {}", opCodeToString(chunk.opCode));
        break;
      }
    }
  }
}

void Prop::nextFrame(U32 sceneFrame) {
  updateState(sceneFrame);

  m_offset.x += m_movePerFrame.x % 2;
  m_offset.y += m_movePerFrame.y % 2;
}

ImageProp::ImageProp(std::vector<Film::Chunk> chunks, std::unique_ptr<Image> image)
  : Prop{std::move(chunks)}, m_image{std::move(image)} {}

void ImageProp::render(const RenderState& renderState) {
  if (m_visible) {
    drawImage(*m_image, renderState, m_offset);
  }
}

AnimationProp::AnimationProp(std::vector<Film::Chunk> chunks, std::unique_ptr<Animation> animation)
  : Prop{std::move(chunks)}, m_animation{std::move(animation)} {}

void AnimationProp::nextFrame(U32 sceneFrame) {
  Prop::nextFrame(sceneFrame);

  m_currentFrame += 1;
  if (m_currentFrame >= m_animation->frames().size()) {
    m_currentFrame = 0;
  }
}

void AnimationProp::render(const RenderState& renderState) {
  if (m_visible) {
    drawImage(m_animation->frames()[m_currentFrame], renderState, m_offset);
  }
}
