#include "lfd/animation.h"

#include <nucleus/Logging.h>
#include <nucleus/Streams/InputStream.h>
#include <nucleus/Streams/OutputStream.h>

#include <cassert>

#define TRACE_READ 0

namespace {

Image readFrame(nu::InputStream* stream) {
  auto length = stream->readU32();

  auto startPosition = stream->getPosition();

#if TRACE_READ
  LOG(Info) << "data size for frame: " << length;
#endif

  Image image;
  image.read(stream, length);

  stream->setPosition(startPosition + length);

  return image;
}

}  // namespace

Animation::~Animation() = default;

void Animation::read(nu::InputStream* stream, MemSize size) {
  auto frameCount = stream->readU16();

#if TRACE_READ
  LOG(Info) << "Animation :: frameCount: " << frameCount;
#endif

  for (U16 i = 0; i < frameCount; ++i) {
    m_frames.emplace_back(readFrame(stream));
  }
}

void Animation::write(nu::OutputStream* stream) {
  assert(false);
}
