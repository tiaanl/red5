#include "lfd/animation.h"

#include <base/logging.h>
#include <base/streams/input_stream.h>

#include <cassert>

#define TRACE_READ 1

namespace {

Image readFrame(base::InputStream* stream) {
  auto dataSize = stream->readU32();

  auto startPosition = stream->getPosition();

#if TRACE_READ
  lg::info("Frame :: dataSize: {}", dataSize);
#endif

  Image image;
  image.read(stream, dataSize);

  stream->setPosition(startPosition + dataSize);

  return image;
}

}  // namespace

Animation::~Animation() = default;

void Animation::read(base::InputStream* stream, MemSize size) {
  auto frameCount = stream->readU16();

#if TRACE_READ
  lg::info("Animation :: frameCount: {}", frameCount);
#endif

  for (U16 i = 0; i < frameCount; ++i) {
    m_frames.emplace_back(readFrame(stream));
  }
}

void Animation::write(base::OutputStream* stream) {
  assert(false);
}
