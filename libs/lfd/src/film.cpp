#include "lfd/film.h"

#include "base/streams/input_stream.h"
#include "base/streams/output_stream.h"

#define TRACE_LOADING 0

namespace {

Film::Chunk readChunk(base::InputStream* stream) {
  auto dataSize = stream->readU16();
  auto opCode = static_cast<OpCode>(stream->readU16());

  U16 varCount = (dataSize) / 2 - 2;

#if TRACE_LOADING
  LOG(Info) << "    Chunk :: dataSize: " << dataSize << ", opCode: " << (U16)opCode << " ("
            << opCodeToString(opCode) << "), varCount: " << varCount;
#endif

  Film::Chunk newChunk;
  newChunk.opCode = opCode;

  for (U16 i = 0; i < varCount; ++i) {
    auto variable = stream->readU16();
    newChunk.variables.push_back(variable);
#if TRACE_LOADING
    LOG(Info) << "      var: " << variable;
#endif
  }

  return newChunk;
}

Film::Block readBlock(base::InputStream* stream) {
  Film::Block result;

  result.type = static_cast<BlockType>(stream->readU32());

  char nameStr[9] = {};
  stream->read(nameStr, 8);
  result.name = std::string_view{nameStr, std::min(std::strlen(nameStr), 8ull)};

  auto length = stream->readU32();
  result.typeIndex = stream->readU16();

  if (result.type == BlockType::End) {
#if TRACE_LOADING
    LOG(Info) << "  Block :: type: " << blockTypeToString(result.type) << ", name: " << result.name
              << ", typeIndex: " << result.typeIndex << ", length: " << length;
#endif

    return result;
  }

  auto numberOfChunks = stream->readU16();
  auto chunksDataSize = stream->readU16();

#if TRACE_LOADING
  LOG(Info) << "  Block :: type: " << blockTypeToString(result.type) << ", name: " << nameStr
            << ", typeIndex: " << result.typeIndex << ", length: " << length
            << ", numberOfChunks: " << numberOfChunks << ", chunkDataSize: " << chunksDataSize;
#endif

  MemSize before = stream->getPosition();

  for (U32 c = 0; c < numberOfChunks; ++c) {
    auto& chunk = result.chunks.emplace_back(readChunk(stream));
  }

  MemSize bytesRead = stream->getPosition() - before;
  //  DCHECK(bytesRead == chunksDataSize)
  //      << "Incorrect amount of bytes read (" << bytesRead << " of " << chunksDataSize << ")";

  return result;
}

void writeChunk(base::OutputStream* stream, const Film::Chunk& chunk) {
  stream->writeU16(static_cast<U16>(chunk.variables.size() + 2) * sizeof(U16));
  stream->writeU16(static_cast<U16>(chunk.opCode));
  for (auto v : chunk.variables) {
    stream->writeU16(v);
  }
}

U16 calculateChunkSize(const Film::Chunk& chunk) {
  return sizeof(U16) + sizeof(U16) + static_cast<U16>(chunk.variables.size()) * sizeof(U16);
}

U32 calculateBlockSize(const Film::Block& block) {
  U32 size = 0;

  for (auto& chunk : block.chunks) {
    size += calculateChunkSize(chunk);
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
  if (block.chunks.empty()) {
    return;
  }

  stream->writeU16(static_cast<U16>(block.chunks.size()));
  U16 chunksDataSize = 0;
  for (auto& chunk : block.chunks) {
    chunksDataSize += calculateChunkSize(chunk);
  }
  stream->writeU16(chunksDataSize);

  for (auto& chunk : block.chunks) {
    writeChunk(stream, chunk);
  }
}

}  // namespace

const char* opCodeToString(OpCode opCode) {
  switch (opCode) {
    case OpCode::End:
      return "End";

    case OpCode::Time:
      return "Time";

    case OpCode::Move:
      return "Move";

    case OpCode::Speed:
      return "Speed";

    case OpCode::Layer:
      return "Layer";

    case OpCode::Frame:
      return "Frame";

    case OpCode::Animation:
      return "Animation";

    case OpCode::Event:
      return "Event";

    case OpCode::Region:
      return "Region";

    case OpCode::Window:
      return "Window";

    case OpCode::Shift:
      return "Shift";

    case OpCode::Display:
      return "Display";

    case OpCode::Orientation:
      return "Orientation";

    case OpCode::Use:
      return "Use";

    case OpCode::Unknown11:
      return "Unknown11";

    case OpCode::Transition:
      return "Transition";

    case OpCode::Unknown12:
      return "Unknown12";

    case OpCode::Loop:
      return "Loop";

    case OpCode::Unknown17:
      return "Unknown";

    case OpCode::Preload:
      return "Preload";

    case OpCode::Sound:
      return "Sound";

    case OpCode::Stereo:
      return "Stereo";
  }

  return "Unknown";
}

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
