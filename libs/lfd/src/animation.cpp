#include "lfd/animation.h"

#include <base/logging.h>
#include <base/streams/input_stream.h>

#include <cassert>

#define TRACE_READ 0

Animation::~Animation() = default;

void Animation::read(base::InputStream* stream, MemSize size) {
  auto frameCount = stream->readU16();

#if TRACE_READ >= 1
  lg::info("Animation :: frameCount: {}", frameCount);
#endif

  for (U16 i = 0; i < frameCount; ++i) {
    auto frameDataSize = stream->readU32();

#if TRACE_READ >= 2
    lg::info("Frame :: frameDataSize = {}", frameDataSize);
#endif
    
    auto& newFrame = m_frames.emplace_back();
    newFrame.read(stream, frameDataSize);
  }
}

void Animation::write(base::OutputStream* stream) {
  assert(false);
}
