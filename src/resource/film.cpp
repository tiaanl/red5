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

    case OpCode::Unknown10:
      return "Unknown10";

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

U32 getChunkSize(OpCode opCode) {
  switch (opCode) {
    case OpCode::Time:
      return sizeof(TimeChunk);

    case OpCode::Move:
      return sizeof(MoveChunk);

    case OpCode::Speed:
      return sizeof(SpeedChunk);

    case OpCode::Layer:
      return sizeof(LayerChunk);

    case OpCode::Animation:
      return sizeof(AnimationChunk);

    case OpCode::Shift:
      return sizeof(ShiftChunk);

    case OpCode::Unknown10:
      return sizeof(Unknown10Chunk);

    case OpCode::Transition:
      return sizeof(TransitionChunk);

    default:
      DCHECK(false);
      return 0;
  }
}

void readChunk(nu::InputStream* stream, U32 chunkIndex, nu::DynamicArray<Chunk>* chunks) {
  auto opCode = OpCode(stream->readU16());

  // LOG(Info) << "    Chunk (" << chunkIndex << ") :: opCode: " << (U16)opCode << " ("
  //           << opCodeToString(opCode) << ")";

  switch (opCode) {
    case OpCode::End:
      break;

    case OpCode::Time:
      chunks->emplaceBack(opCode).element().time = {stream->readU16()};
      break;

    case OpCode::Move:
      chunks->emplaceBack(opCode).element().move = {stream->readU16()};
      break;

    case OpCode::Speed:
      chunks->emplaceBack(opCode).element().speed = {stream->readU16()};
      break;

    case OpCode::Layer:
      chunks->emplaceBack(opCode).element().layer = {stream->readU16(), stream->readU16()};
      break;

    case OpCode::Frame:
      DCHECK(false);
      break;

    case OpCode::Animation:
      chunks->emplaceBack(opCode).element().animation = {stream->readU16(), stream->readU16(),
                                                         stream->readU16()};
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
      chunks->emplaceBack(opCode).element().shift = {stream->readU16(), stream->readU16(),
                                                     stream->readU16(), stream->readU16(),
                                                     stream->readU16()};
      break;

    case OpCode::Display:
      DCHECK(false);
      break;

    case OpCode::Orientation:
      DCHECK(false);
      break;

    case OpCode::Use:
      DCHECK(false);
      break;

    case OpCode::Unknown10:
      chunks->emplaceBack(opCode).element().unknown10 = {
          stream->readU16(), stream->readU16(), stream->readU16(), stream->readU16(),
          stream->readU16(), stream->readU16(), stream->readU16()};
      break;

    case OpCode::Unknown11:
      break;

    case OpCode::Transition:
      chunks->emplaceBack(opCode).element().transition = {
          stream->readU16(), stream->readU16(), stream->readU16(), stream->readU16(),
          stream->readU16(), stream->readU16(), stream->readU16(), stream->readU16()};
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

void readBlock(nu::InputStream* stream, U32 index, nu::DynamicArray<Block>* blocks) {
  auto type = stream->readU32();

  char name[9] = {};
  stream->read(name, 8);

  auto length = stream->readU32();
  auto typeIndex = stream->readU16();

  if (BlockType(type) == BlockType::End) {
    // LOG(Info) << "  Block (" << index << ") :: type: " << blockTypeToString((BlockType)type)
    //           << ", name: " << name << ", length: " << length << ", typeIndex: " << typeIndex;

    blocks->emplaceBack(static_cast<BlockType>(type), typeIndex,
                        nu::StringView{name, std::min(std::strlen(name), 8ull)});
    return;
  }

  auto numberOfChunks = stream->readU16();
  auto chunkDataSize = stream->readU16();

  // LOG(Info) << "  Block :: type: " << blockTypeToString((BlockType)type) << ", name: " << name
  //           << ", length: " << length << ", typeIndex: " << typeIndex
  //           << ", numberOfChunks: " << numberOfChunks << ", chunkDataSize: " << chunkDataSize;

  MemSize before = stream->getPosition();

  auto ir = blocks->emplaceBack(static_cast<BlockType>(type), typeIndex,
                                nu::StringView{name, std::min(std::strlen(name), 8ull)});

  for (U32 c = 0; c < numberOfChunks; ++c) {
    readChunk(stream, c, &ir.element().chunks);
  }

  MemSize bytesRead = stream->getPosition() - before;
  DCHECK(bytesRead == chunkDataSize)
      << "Incorrect amount of bytes read (" << bytesRead << " of " << chunkDataSize << ")";
}

nu::ScopedPtr<Film> read(nu::InputStream* stream) {
  stream->skip(sizeof(U16));  // reserved
  auto numberOfFrames = stream->readU16();
  U16 numberOfBlocks = stream->readU16() + 1u;  // 0 indexed.

  // LOG(Info) << "Film :: reserved: " << reserved << ", numberOfFrames: " << numberOfFrames
  //           << ", numberOfBlocks: " << numberOfBlocks;

  auto result = nu::makeScopedPtr<Film>();
  result->numberOfFrames = numberOfFrames;

  for (U16 b = 0; b < numberOfBlocks; ++b) {
    readBlock(stream, b, &result->blocks);
  }

  return result;
}

void writeChunk(nu::OutputStream* stream, const Chunk& chunk) {
  stream->writeU16(static_cast<U16>(chunk.opCode));

  switch (chunk.opCode) {
    case OpCode::Time:
      stream->writeU16(chunk.time.p1);
      break;

    case OpCode::Move:
      stream->writeU16(chunk.move.p1);
      break;

    case OpCode::Speed:
      stream->writeU16(chunk.speed.p1);
      break;

    case OpCode::Layer:
      stream->writeU16(chunk.layer.layer);
      stream->writeU16(chunk.layer.p1);
      break;

    case OpCode::Animation:
      stream->writeU16(chunk.animation.p1);
      stream->writeU16(chunk.animation.p2);
      stream->writeU16(chunk.animation.p3);
      break;

    case OpCode::Shift:
      stream->writeU16(chunk.shift.p1);
      stream->writeU16(chunk.shift.p2);
      stream->writeU16(chunk.shift.p3);
      stream->writeU16(chunk.shift.p4);
      stream->writeU16(chunk.shift.p5);
      break;

    case OpCode::Unknown10:
      stream->writeU16(chunk.unknown10.p1);
      stream->writeU16(chunk.unknown10.p2);
      stream->writeU16(chunk.unknown10.p3);
      stream->writeU16(chunk.unknown10.p4);
      stream->writeU16(chunk.unknown10.p5);
      stream->writeU16(chunk.unknown10.p6);
      stream->writeU16(chunk.unknown10.p7);
      break;

    case OpCode::Transition:
      stream->writeU16(chunk.transition.p1);
      stream->writeU16(chunk.transition.p2);
      stream->writeU16(chunk.transition.p3);
      stream->writeU16(chunk.transition.p4);
      stream->writeU16(chunk.transition.p5);
      stream->writeU16(chunk.transition.p6);
      stream->writeU16(chunk.transition.p7);
      stream->writeU16(chunk.transition.p8);
      break;

    default:
      DCHECK(false);
      break;
  }
}

void writeBlock(nu::OutputStream* stream, const Block& block) {
  stream->writeU32(static_cast<U32>(block.type));

  U8 nameStr[8] = {};
  std::memcpy(nameStr, block.name.data(), std::min(sizeof(nameStr), block.name.length()));
  stream->write(nameStr, sizeof(nameStr));

  U32 chunkDataSize = 0;
  for (auto& chunk : block.chunks) {
    chunkDataSize += sizeof(U16) + getChunkSize(chunk.opCode);
  }
  // length
  stream->writeU32(chunkDataSize + 18 + (block.chunks.isEmpty() ? 0 : 4));

  stream->writeU16(block.typeIndex);

  // If there are no chunks, then we don't write any data for it.
  if (block.chunks.isEmpty()) {
    return;
  }

  stream->writeU16(static_cast<U16>(block.chunks.size()));
  stream->writeU16(chunkDataSize);

  for (auto& chunk : block.chunks) {
    writeChunk(stream, chunk);
  }
}

void write(nu::OutputStream* stream, const Film& film) {
  stream->writeU16(4);  // reserved
  stream->writeU16(film.numberOfFrames);
  stream->writeU16(static_cast<U16>(film.blocks.size() - 1u));

  for (auto& block : film.blocks) {
    writeBlock(stream, block);
  }
}

}  // namespace film
