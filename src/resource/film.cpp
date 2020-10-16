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

void readChunk(nu::InputStream* stream, U32 chunkIndex, nu::DynamicArray<Chunk>* chunks) {
  auto dataSize = stream->readU16();
  auto opCode = OpCode(stream->readU16());

  U16 varCount = (dataSize) / 2 - 2;

  LOG(Info) << "    Chunk (" << chunkIndex << ") :: dataSize: " << dataSize
            << ", opCode: " << (U16)opCode << " (" << opCodeToString(opCode)
            << "), varCount: " << varCount;

  auto ir = chunks->emplaceBack(opCode);

  for (U16 i = 0; i < std::min(varCount, (U16)10); ++i) {
    U16 var = stream->readU16();
    ir.element().vars.pushBack(var);
    LOG(Info) << "      var: " << var;
  }

#if 0
  auto before = stream->getPosition();

  switch (opCode) {
    case OpCode::End: {
      break;
    }

    case OpCode::Time: {
      LOG(Info) << "      p: " << stream->readU16();
      break;
    }

    case OpCode::Move: {
      //      U16 p1 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      chunks->emplaceBack(opCode).element().move = {p1};
      break;
    }

    case OpCode::Speed: {
      //      U16 p1 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      chunks->emplaceBack(opCode).element().speed = {p1};
      break;
    }

    case OpCode::Layer: {
      //      U16 p1 = stream->readU16();
      //      U16 p2 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      LOG(Info) << "      p2 = " << p2;
      //      chunks->emplaceBack(opCode).element().layer = {p1, p2};
      break;
    }

    case OpCode::Frame: {
      DCHECK(false);
      break;
    }

    case OpCode::Animation: {
      //      U16 p1 = stream->readU16();
      //      U16 p2 = stream->readU16();
      //      U16 p3 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      LOG(Info) << "      p2 = " << p2;
      //      LOG(Info) << "      p3 = " << p3;
      //      chunks->emplaceBack(opCode).element().animation = {p1, p2, p3};
      break;
    }

    case OpCode::Event: {
      DCHECK(false);
      break;
    }

    case OpCode::Region: {
      DCHECK(false);
      break;
    }

    case OpCode::Window: {
      DCHECK(false);
      break;
    }

    case OpCode::Shift: {
      //      U16 p1 = stream->readU16();
      //      U16 p2 = stream->readU16();
      //      U16 p3 = stream->readU16();
      //      U16 p4 = stream->readU16();
      //      U16 p5 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      LOG(Info) << "      p2 = " << p2;
      //      LOG(Info) << "      p3 = " << p3;
      //      LOG(Info) << "      p4 = " << p4;
      //      LOG(Info) << "      p5 = " << p5;
      //      chunks->emplaceBack(opCode).element().shift = {p1, p2, p3, p4, p5};
      break;
    }

    case OpCode::Display: {
      DCHECK(false);
      break;
    }

    case OpCode::Orientation: {
      DCHECK(false);
      break;
    }

    case OpCode::Use: {
      DCHECK(false);
      break;
    }

    case OpCode::Unknown10: {
      //      U16 p1 = stream->readU16();
      //      U16 p2 = stream->readU16();
      //      U16 p3 = stream->readU16();
      //      U16 p4 = stream->readU16();
      //      U16 p5 = stream->readU16();
      //      U16 p6 = stream->readU16();
      //      U16 p7 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      LOG(Info) << "      p2 = " << p2;
      //      LOG(Info) << "      p3 = " << p3;
      //      LOG(Info) << "      p4 = " << p4;
      //      LOG(Info) << "      p5 = " << p5;
      //      LOG(Info) << "      p6 = " << p6;
      //      LOG(Info) << "      p7 = " << p7;
      //      chunks->emplaceBack(opCode).element().unknown10 = {p1, p2, p3, p4, p5, p6, p7};
      break;
    }

    case OpCode::Unknown11: {
      break;
    }

    case OpCode::Transition: {
      //      U16 p1 = stream->readU16();
      //      U16 p2 = stream->readU16();
      //      U16 p3 = stream->readU16();
      //      U16 p4 = stream->readU16();
      //      U16 p5 = stream->readU16();
      //      U16 p6 = stream->readU16();
      //      U16 p7 = stream->readU16();
      //      U16 p8 = stream->readU16();
      //      LOG(Info) << "      p1 = " << p1;
      //      LOG(Info) << "      p2 = " << p2;
      //      LOG(Info) << "      p3 = " << p3;
      //      LOG(Info) << "      p4 = " << p4;
      //      LOG(Info) << "      p5 = " << p5;
      //      LOG(Info) << "      p6 = " << p6;
      //      LOG(Info) << "      p7 = " << p7;
      //      LOG(Info) << "      p8 = " << p8;
      //      chunks->emplaceBack(opCode).element().transition = {p1, p2, p3, p4, p5, p6, p7, p8};
      break;
    }

    case OpCode::Unknown12: {
      DCHECK(false);
      break;
    }

    case OpCode::Loop: {
      DCHECK(false);
      break;
    }

    case OpCode::Unknown17: {
      DCHECK(false);
      break;
    }

    case OpCode::Preload: {
      DCHECK(false);
      break;
    }

    case OpCode::Sound: {
      DCHECK(false);
      break;
    }

    case OpCode::Stereo: {
      DCHECK(false);
      break;
    }
  }

  auto varsRead = (stream->getPosition() - before) / 2;
//  DCHECK(varsRead == varCount) << "Did not read enough variables. (" << varsRead << " out of "
//                               << varCount << ")";
#endif  // 0
}

void readBlock(nu::InputStream* stream, U32 index, nu::DynamicArray<Block>* blocks) {
  auto type = stream->readU32();

  char name[9] = {};
  stream->read(name, 8);

  auto length = stream->readU32();
  auto typeIndex = stream->readU16();

  if (BlockType(type) == BlockType::End) {
    LOG(Info) << "  Block (" << index << ") :: type: " << blockTypeToString((BlockType)type)
              << ", name: " << name << ", length: " << length << ", typeIndex: " << typeIndex;

    blocks->emplaceBack(static_cast<BlockType>(type), typeIndex,
                        nu::StringView{name, std::min(std::strlen(name), 8ull)});
    return;
  }

  auto numberOfChunks = stream->readU16();
  auto chunksDataSize = stream->readU16();

  LOG(Info) << "  Block (" << index << ") :: type: " << blockTypeToString((BlockType)type)
            << ", name: " << name << ", length: " << length << ", typeIndex: " << typeIndex
            << ", numberOfChunks: " << numberOfChunks << ", chunkDataSize: " << chunksDataSize;

  MemSize before = stream->getPosition();

  auto ir = blocks->emplaceBack(static_cast<BlockType>(type), typeIndex,
                                nu::StringView{name, std::min(std::strlen(name), 8ull)});

  for (U32 c = 0; c < numberOfChunks; ++c) {
    //    auto chunkDataSize = stream->readU16();
    //    auto opCode = stream->readU16();
    //    auto variableCount = (chunkDataSize - sizeof(U16) - sizeof(U16)) / 2;  // size - opcode
    //
    //    LOG(Info) << "chunkDataSize: " << chunkDataSize << ", opCode: " << opCode
    //              << ", variableCount: " << variableCount;
    //
    //    for (U16 v = 0; v < variableCount; ++v) {
    //      LOG(Info) << "var: " << stream->readU16();
    //    }

    readChunk(stream, c, &ir.element().chunks);
  }

  MemSize bytesRead = stream->getPosition() - before;
  DCHECK(bytesRead == chunksDataSize)
      << "Incorrect amount of bytes read (" << bytesRead << " of " << chunksDataSize << ")";
}

nu::ScopedPtr<Film> read(nu::InputStream* stream) {
  stream->skip(sizeof(U16));  // reserved
  auto numberOfFrames = stream->readU16();
  U16 numberOfBlocks = stream->readU16() + 1u;  // 0 indexed.

  LOG(Info) << "Film :: numberOfFrames: " << numberOfFrames
            << ", numberOfBlocks: " << numberOfBlocks;

  auto result = nu::makeScopedPtr<Film>();
  result->numberOfFrames = numberOfFrames;

  for (U16 b = 0; b < numberOfBlocks; ++b) {
    readBlock(stream, b, &result->blocks);
  }

  return result;
}

void writeChunk(nu::OutputStream* stream, const Chunk& chunk) {
  stream->writeU16((U16)(chunk.vars.size() + 2) * sizeof(U16));
  stream->writeU16(static_cast<U16>(chunk.opCode));
  for (auto v : chunk.vars) {
    stream->writeU16(v);
  }
}

U16 chunkSize(const Chunk& chunk) {
  return sizeof(U16) + sizeof(U16) + (U16)chunk.vars.size() * sizeof(U16);
}

U32 blockSize(const Block& block) {
  U32 size = 0;

  for (auto& chunk : block.chunks) {
    size += chunkSize(chunk);
  }

  return size;
}

void writeBlock(nu::OutputStream* stream, const Block& block) {
  stream->writeU32(static_cast<U32>(block.type));  // 10

  U8 nameStr[8] = {};
  std::memcpy(nameStr, block.name.data(), std::min(sizeof(nameStr), block.name.length()));
  stream->write(nameStr, sizeof(nameStr));  // 18

  U32 bs = blockSize(block);
  if (block.type != BlockType::End) {
    bs += 22;
  } else {
    bs += 18;
  }
  stream->writeU32(bs);

  stream->writeU16(block.typeIndex);

  // If there are no chunks, then we don't write any data for it.
  if (block.chunks.isEmpty()) {
    return;
  }

  stream->writeU16(static_cast<U16>(block.chunks.size()));
  U16 chunksDataSize = 0;
  for (auto& chunk : block.chunks) {
    chunksDataSize += chunkSize(chunk);
  }
  stream->writeU16(chunksDataSize);

  for (auto& chunk : block.chunks) {
    writeChunk(stream, chunk);
  }
}

void write(nu::OutputStream* stream, const Film& film) {
  stream->writeU16(4);                                          // 2  // reserved
  stream->writeU16(film.numberOfFrames);                        // 4
  stream->writeU16(static_cast<U16>(film.blocks.size() - 1u));  // 6

  for (auto& block : film.blocks) {
    writeBlock(stream, block);
  }
}

}  // namespace film
