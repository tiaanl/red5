#include "resource/film.h"

#include <nucleus/Logging.h>

namespace film {

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
  switch (blockType) {
    case BlockType::Anim:
      return "Anim";

    case BlockType::Cust:
      return "Cust";

    case BlockType::Delt:
      return "Delt";

    case BlockType::End:
      return "End";

    case BlockType::Pltt:
      return "Pltt";

    case BlockType::View:
      return "View";

    case BlockType::Voic:
      return "Voic";

    default:
      return "Unknown";
  }
}

void readChunk(nu::InputStream* stream, U32 chunkIndex) {
  auto opCode = OpCode(stream->readU16());

  LOG(Info) << "    Chunk (" << chunkIndex << ") :: opCode: " << (U16)opCode << " ("
            << opCodeToString(opCode) << ")";

  switch (opCode) {
    case OpCode::End:
      break;

    case OpCode::Time:
      LOG(Info) << "      frameNum:    " << stream->readU16();
      break;

    case OpCode::Move:
      LOG(Info) << "      ??:          " << stream->readU16();
      break;

    case OpCode::Speed:
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      break;

    case OpCode::Layer:
      LOG(Info) << "      layer:       " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readI16();
      break;

    case OpCode::Frame:
      DCHECK(false);
      break;

    case OpCode::Animation:
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      // LOG(Info) << "      ??:          " << stream->readU16();
      break;

    case OpCode::Event:
      DCHECK(false);
      break;

    case OpCode::Region:
      DCHECK(false);
      break;

    case OpCode::Window:
      DCHECK(false);
      break;

    case OpCode::Shift:
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      LOG(Info) << "      ??:          " << stream->readU16();
      break;

    case OpCode::Display:
      LOG(Info) << "      ??:          " << stream->readU16();
      break;

    case OpCode::Orientation:
      DCHECK(false);
      break;

    case OpCode::Use:
      DCHECK(false);
      break;

    case OpCode::Unknown11:
      break;

    case OpCode::Transition:
      DCHECK(false);
      break;

    case OpCode::Unknown12:
      DCHECK(false);
      break;

    case OpCode::Loop:
      DCHECK(false);
      break;

    case OpCode::Unknown17:
      DCHECK(false);
      break;

    case OpCode::Preload:
      DCHECK(false);
      break;

    case OpCode::Sound:
      DCHECK(false);
      break;

    case OpCode::Stereo:
      DCHECK(false);
      break;
  }
}

void readBlock(nu::InputStream* stream, U32 index) {
  auto type = stream->readU32();

  char name[9] = {};
  stream->read(name, 8);

  auto length = stream->readU32();
  auto typeIndex = stream->readU16();

  if (BlockType(type) == BlockType::End) {
    LOG(Info) << "  Block (" << index << ") :: type: " << blockTypeToString((BlockType)type)
              << ", name: " << name << ", length: " << length << ", typeIndex: " << typeIndex;
    return;
  }

  auto numberOfChunks = stream->readU16();
  auto chunkDataSize = stream->readU16();

  LOG(Info) << "  Block :: type: " << blockTypeToString((BlockType)type) << ", name: " << name
            << ", length: " << length << ", typeIndex: " << typeIndex
            << ", numberOfChunks: " << numberOfChunks << ", chunkDataSize: " << chunkDataSize;

  MemSize before = stream->getPosition();

  for (U32 c = 0; c < numberOfChunks; ++c) {
    readChunk(stream, c);
  }

  MemSize bytesRead = stream->getPosition() - before;
  DCHECK(bytesRead == chunkDataSize)
      << "Incorrect amount of bytes read (" << bytesRead << " of " << chunkDataSize << ")";
}

bool load(nu::InputStream* stream) {
  auto reserved = stream->readU16();
  auto numberOfFrames = stream->readU16();
  auto numberOfBlocks = stream->readU16() + 1;  // 0 indexed.

  LOG(Info) << "Film :: reserved: " << reserved << ", numberOfFrames: " << numberOfFrames
            << ", numberOfBlocks: " << numberOfBlocks;

  for (U16 b = 0; b < numberOfBlocks; ++b) {
    readBlock(stream, b);
  }

  return true;
}

}  // namespace film
