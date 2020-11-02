#include "lfd/image.h"

#include <base/streams/input_stream.h>
#include <base/streams/output_stream.h>

#define TRACE_READ 0

namespace {

using Line = Image::Line;

bool readLine(base::InputStream* stream, std::vector<Line>* lines) {
  U16 dataSize = stream->readU16();

  if (dataSize == 0) {
    return false;
  }

  auto left = stream->readU16();
  auto top = stream->readU16();

  bool isCompressed = (dataSize & 1u) == 0;
  U16 lineLength = dataSize >> 1u;

#if TRACE_READ >= 2
  spdlog::info("Line :: dataSize: {}, left: {}, top: {}, isCompressed: {}, lineLength: {}",
               dataSize, left, top, isCompressed, lineLength);
#endif

  std::vector<U8> indices;

  if (isCompressed) {
    for (U16 k = 0; k < lineLength; ++k) {
      indices.emplace_back(stream->readU8());
    }
  } else {
    while (indices.size() < lineLength) {
      U8 opCode = stream->readU8();

      if ((opCode % 2) == 1) {
        U8 repeatCount = opCode >> 1u;
        U8 index = stream->readU8();
#if TRACE_READ >= 3
        LOG(Info) << "Odd opCode :: repeatCount: " << (U32)repeatCount << ", index: " << (U32)index;
#endif
        for (U8 k = 0; k < repeatCount; ++k) {
          indices.emplace_back(index);
        }
      } else {
        U8 indexCount = opCode >> 1u;
#if TRACE_READ >= 3
        LOG(Info) << "Even opCode :: indexCount: " << (U32)indexCount;
#endif
        for (U8 k = 0; k < indexCount; ++k) {
          U8 index = stream->readU8();
#if TRACE_READ
          // LOG(Info) << "  index: " << (U32)index;
#endif
          indices.emplace_back(index);
        }
      }
    }
  }

  lines->emplace_back(left, top, std::move(indices));

  return true;
}

}  // namespace

Image::~Image() = default;

void Image::read(base::InputStream* stream, MemSize size) {
  m_left = stream->readU16();
  m_top = stream->readU16();
  m_right = stream->readU16();
  m_bottom = stream->readU16();

#if TRACE_READ >= 1
  spdlog::info("Image ({}) :: left: {}, top: {}, right: {}, bottom: {}", size, m_left, m_top,
               m_right, m_bottom);
#endif

  for (;;) {
    if (!readLine(stream, &m_lines)) {
      break;
    }
  }
}

void Image::write(base::OutputStream* stream) {
  stream->writeU16(m_left);
  stream->writeU16(m_top);
  stream->writeU16(m_right);
  stream->writeU16(m_bottom);
}
