#pragma once

#include <string>
#include <vector>

#include "base/platform.h"
#include "lfd/resource_type.h"
#include "resource.h"

namespace base {
class InputStream;
class OutputStream;
}  // namespace base

enum class OpCode : U16 {
  End = 0x02,          // Marker for the end of a `Block`
  Time = 0x03,         // Sets the time to execute the following commands.
  Move = 0x04,         // Move the resource to a new location for animation purposes.
  Speed = 0x05,        // The speed at which the `Move` OpCode command is executed.
  Layer = 0x06,        // Establishes the drawing order for the resource.
  Frame = 0x07,        // Display the resource beginning from a specific frame.
  Animation = 0x08,    // The direction and rate of the display for an animation resource.
  Event = 0x09,        // Executable command.
  Region = 0x0A,       // Executable-defined screen location for `Click` events.
  Window = 0x0B,       // Displays a portion of the resource.
  Shift = 0x0C,        // Offset the image location.
  Display = 0x0D,      // Show or hide the resource.
  Orientation = 0x0E,  // Flip the X/Y display of the resource.
  Use = 0x0F,          // Marker to activate the palette.
  Unknown11 = 0x11,    // Unknown.
  Transition = 0x12,   // Method of clearing the screen and loading the new `View`.
  Unknown12 = 0x13,    // Unknown.
  Loop = 0x14,         // Appears to stop sounds internally set to repeat.
  Unknown17 = 0x17,    // Unknown.
  Preload = 0x18,      // Possibly defines file the sound is located in.
  Sound = 0x19,        // Determines playback controls.
  Stereo = 0x1C,       // Determines stereo playback controls.
};

enum class BlockType : U32 {
  Anim = static_cast<U32>(ResourceType::Animation),
  Cust = 0x54535543,
  Delt = static_cast<U32>(ResourceType::Image),
  Pltt = static_cast<U32>(ResourceType::Palette),
  View = 0x57454956,
  Voic = static_cast<U32>(ResourceType::Voice),

  // Marks the end of the `Film`.
  End = 0x00444E45,
};

class Film : public Resource {
public:
  struct Chunk {
    OpCode opCode;
    std::vector<I16> variables;
  };

  struct Block {
    BlockType type;
    U16 typeIndex;
    std::string name;
    std::vector<Chunk> chunks;
  };

  ~Film() override;

  U32 frameCount() const {
    return m_frameCount;
  }

  const std::vector<Block>& blocks() const {
    return m_blocks;
  }

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  U32 m_frameCount;
  std::vector<Block> m_blocks;
};

const char* opCodeToString(OpCode opCode);
const char* blockTypeToString(BlockType blockType);
