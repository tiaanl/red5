#include "lfd/image.h"

#include <nucleus/Logging.h>
#include <nucleus/Streams/InputStream.h>
#include <nucleus/Streams/OutputStream.h>

#define TRACE_READ 0

namespace {

using Line = Image::Line;

Line readLine(nu::InputStream* stream) {
  Line result;

  U16 dataSize = stream->readU16();
  result.left = stream->readU16();
  result.top = stream->readU16();

  bool isCompressed = (dataSize & 1u) == 0;
  U16 lineLength = dataSize >> 1u;

#if TRACE_READ
  LOG(Info) << "Line :: dataSize: " << dataSize << ", left: " << result.left << ", top: " << result.top
            << ", isCompressed: " << isCompressed << ", lineLength: " << lineLength;
#endif

  if (isCompressed) {
    for (U16 k = 0; k < lineLength; ++k) {
      result.indices.emplace_back(stream->readU8());
    }
  } else {
    while (result.indices.size() < lineLength) {
      U8 opCode = stream->readU8();

      if ((opCode % 2) == 1) {
        U8 repeatCount = opCode >> 1u;
        U8 index = stream->readU8();
#if TRACE_READ
        LOG(Info) << "Odd opCode :: repeatCount: " << (U32)repeatCount << ", index: " << (U32)index;
#endif
        for (U8 k = 0; k < repeatCount; ++k) {
          result.indices.emplace_back(index);
        }
      } else {
        U8 indexCount = opCode >> 1u;
#if TRACE_READ
        LOG(Info) << "Even opCode :: indexCount: " << (U32)indexCount;
#endif
        for (U8 k = 0; k < indexCount; ++k) {
          U8 index = stream->readU8();
#if TRACE_READ
          // LOG(Info) << "  index: " << (U32)index;
#endif
          result.indices.emplace_back(index);
        }
      }
    }
  }

  return result;
}

}  // namespace

Image::~Image() = default;

void Image::read(nu::InputStream* stream, MemSize size) {
  auto startPosition = stream->getPosition();

  m_left = stream->readU16();
  m_top = stream->readU16();
  m_right = stream->readU16();
  m_bottom = stream->readU16();

#if TRACE_READ
  LOG(Info) << "Image :: left: " << m_left << ", top: " << m_top << ", right: " << m_right
            << ", bottom: " << m_bottom;
#endif

  while (stream->getPosition() - startPosition < size) {
    m_lines.emplace_back(readLine(stream));
  }

//  auto bytesRead = stream->getPosition() - startPosition;
//  if (bytesRead != size) {
//    LOG(Fatal) << "Image resource not fully read.  (" << bytesRead << " out of " << size << ")";
//  }
}

void Image::write(nu::OutputStream* stream) {
  stream->writeU16(m_left);
  stream->writeU16(m_top);
  stream->writeU16(m_right);
  stream->writeU16(m_bottom);
}
