#include "resource/film.h"

#include <assert.h>
#include <stdio.h>

#include "utils/stream.h"

namespace film {

const char* op_code_to_string(OpCode op_code) {
  switch (op_code) {
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

    case OpCode::Unknown:
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

const char* block_type_to_string(BlockType block_type) {
  switch (block_type) {
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

void read_chunk(InputStream* stream) {
  auto op_code = OpCode(read_u16(stream));

  printf("    Chunk :: op_code: %d (%s)\n", (uint16_t)op_code, op_code_to_string(op_code));

  switch (op_code) {
    case OpCode::End:
      break;

    case OpCode::Time:
      printf("      variable: %d\n", read_u16(stream));
      break;

    case OpCode::Move:
      printf("      variable: %d\n", read_u16(stream));
      break;

    case OpCode::Layer:
      printf("      variable: %d\n", read_u16(stream));
      printf("      variable: %d\n", read_u16(stream));
      break;

    case OpCode::Animation:
      printf("      variable: %d\n", read_u16(stream));
      printf("      variable: %d\n", read_u16(stream));
      printf("      variable: %d\n", read_u16(stream));
      printf("      variable: %d\n", read_u16(stream));
      break;

    case OpCode::Display:
      printf("      variable: %d\n", read_u16(stream));
      break;

    case OpCode::Unknown11:
      break;

    default:
      assert(false);
  }
}

void read_block(InputStream* stream) {
  auto type = read_u32(stream);

  char name[9] = {};
  read(stream, name, 8);

  auto length = read_u32(stream);
  auto type_index = read_u16(stream);

  if (BlockType(type) == BlockType::End) {
    printf("  Block :: type: %s, name: %s, length: %d, type_index: %d\n",
           block_type_to_string((BlockType)type), name, length, type_index);
    return;
  }

  auto number_of_chunks = read_u16(stream);
  auto chunk_data_size = read_u16(stream);

  printf(
      "  Block :: type: %s, name: %s, length: %d, type_index: %d, number_of_chunks: %d, "
      "chunk_data_size: %d\n",
      block_type_to_string((BlockType)type), name, length, type_index, number_of_chunks,
      chunk_data_size);

  uint8_t* before = stream->current;

  for (size_t c = 0; c < number_of_chunks && c < 20; ++c) {
    read_chunk(stream);
  }

  printf("    bytes advanced: %lld\n", stream->current - before);
  // advance(stream, chunk_data_length);
}

bool load(Data data) {
  auto stream = stream_from_data(data);

  auto reserved = read_u16(&stream);
  auto number_of_frames = read_u16(&stream);
  auto number_of_blocks = read_u16(&stream) + 1;  // 0 indexed.

  printf("Film :: reserved: %d, number_of_frames: %d, number_of_blocks: %d\n", reserved,
         number_of_frames, number_of_blocks);

  for (size_t i = 0; i < number_of_blocks; ++i) {
    read_block(&stream);
  }

  return true;
}

}  // namespace film
