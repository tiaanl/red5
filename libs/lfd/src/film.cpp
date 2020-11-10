#include "lfd/film.h"

#include <base/streams/input_stream.h>
#include <base/streams/output_stream.h>

#define TRACE_LOADING 0

namespace lfd {

namespace {

KeyFrame readKeyFrame(base::InputStream* stream) {
  auto dataSize = stream->readU16();
  auto opCode = static_cast<OpCode>(stream->readU16());

  U16 varCount = (dataSize) / 2 - 2;

#if TRACE_LOADING
  LOG(Info) << "    KeyFrame :: dataSize: " << dataSize << ", opCode: " << (U16)opCode << " ("
            << opCodeToString(opCode) << "), varCount: " << varCount;
#endif

  KeyFrame::Variables variables;
  for (U16 i = 0; i < varCount; ++i) {
    auto variable = stream->readI16();
    variables.push_back(variable);
#if TRACE_LOADING
    LOG(Info) << "      var: " << variable;
#endif
  }

  return KeyFrame{opCode, std::move(variables)};
}

Film::Block readBlock(base::InputStream* stream) {
  Film::Block result;

  result.type = static_cast<BlockType>(stream->readU32());

  char nameStr[9] = {};
  stream->read(nameStr, 8);
  result.name = std::string_view{nameStr, std::min<MemSize>(std::strlen(nameStr), 8)};

  stream->readU32();  // length
  result.typeIndex = stream->readU16();

  if (result.type == BlockType::End) {
#if TRACE_LOADING
    LOG(Info) << "  Block :: type: " << blockTypeToString(result.type) << ", name: " << result.name
              << ", typeIndex: " << result.typeIndex << ", length: " << length;
#endif

    return result;
  }

  auto numberOfKeyFrames = stream->readU16();
  stream->readU16();  // chunksDataSize

#if TRACE_LOADING
  LOG(Info) << "  Block :: type: " << blockTypeToString(result.type) << ", name: " << nameStr
            << ", typeIndex: " << result.typeIndex << ", length: " << length
            << ", numberOfKeyFrames: " << numberOfKeyFrames
            << ", chunkDataSize: " << chunksDataSize;
#endif

  for (U32 c = 0; c < numberOfKeyFrames; ++c) {
    result.keyFrames.emplace_back(readKeyFrame(stream));
  }

  return result;
}

void writeKeyFrame(base::OutputStream* stream, const KeyFrame& keyFrame) {
  stream->writeU16(static_cast<U16>(keyFrame.variables.size() + 2) * sizeof(U16));
  stream->writeU16(static_cast<U16>(keyFrame.opCode));
  for (auto v : keyFrame.variables) {
    stream->writeU16(v);
  }
}

U16 calculateKeyFrameSize(const KeyFrame& keyFrame) {
  return sizeof(U16) + sizeof(U16) + static_cast<U16>(keyFrame.variables.size()) * sizeof(U16);
}

U32 calculateBlockSize(const Film::Block& block) {
  U32 size = 0;

  for (auto& keyFrame : block.keyFrames) {
    size += calculateKeyFrameSize(keyFrame);
  }

  return size;
}

void writeBlock(base::OutputStream* stream, const Film::Block& block) {
  stream->writeU32(static_cast<U32>(block.type));

  U8 nameStr[8] = {};
  std::memcpy(nameStr, block.name.data(), std::min(sizeof(nameStr), block.name.length()));
  stream->write(nameStr, sizeof(nameStr));

  U32 bs = calculateBlockSize(block);
  if (block.type != BlockType::End) {
    bs += 22;
  } else {
    bs += 18;
  }
  stream->writeU32(bs);

  stream->writeU16(block.typeIndex);

  // If there are no chunks, then we don't write any data for it.
  if (block.keyFrames.empty()) {
    return;
  }

  stream->writeU16(static_cast<U16>(block.keyFrames.size()));
  U16 chunksDataSize = 0;
  for (auto& keyFrame : block.keyFrames) {
    chunksDataSize += calculateKeyFrameSize(keyFrame);
  }
  stream->writeU16(chunksDataSize);

  for (auto& keyFrame : block.keyFrames) {
    writeKeyFrame(stream, keyFrame);
  }
}

}  // namespace

const char* blockTypeToString(BlockType blockType) {
  const char* resourceTypeStr = resourceTypeToString(static_cast<ResourceType>(blockType));
  if (resourceTypeStr != nullptr) {
    return resourceTypeStr;
  }

  switch (blockType) {
    case BlockType::Cust:
      return "Custom";

    case BlockType::View:
      return "View";

    case BlockType::End:
      return "End";

    default:
      return nullptr;
  }
}

Film::~Film() = default;

void Film::read(base::InputStream* stream, MemSize size) {
  m_blocks.clear();

  stream->advance(sizeof(U16));  // reserved
  m_frameCount = stream->readU16();
  U16 numberOfBlocks = stream->readU16() + 1u;  // 0 indexed.

#if TRACE_LOADING
  LOG(Info) << "Film :: numberOfFrames: " << m_frameCount << ", numberOfBlocks: " << numberOfBlocks;
#endif

  for (U16 b = 0; b < numberOfBlocks; ++b) {
    m_blocks.emplace_back(readBlock(stream));
  }
}

void Film::write(base::OutputStream* stream) {
  stream->writeU16(4);
  stream->writeU16(m_frameCount);
  stream->writeU16(static_cast<U16>(m_blocks.size() - 1u));

  for (auto& block : m_blocks) {
    writeBlock(stream, block);
  }
}

}  // namespace lfd
