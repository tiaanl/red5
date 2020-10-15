#ifndef RESOURCE_FILM_H_
#define RESOURCE_FILM_H_

#include <nucleus/Containers/DynamicArray.h>
#include <nucleus/Memory/ScopedPtr.h>
#include <nucleus/Streams/InputStream.h>
#include <nucleus/Text/DynamicString.h>

#include "lfd.h"

// HEADER
// - U16 reserved
// - U16 number of frames
// - U16 number of blocks
// - blocks
//   - U32 type
//   - U8*8 name
//   - U32 length
//   - U16 type index
//   - U16 number of chunks
//   - U16 chunk data size
//   - chunks
//     - U16 op code
//     - U16*? variables

namespace film {

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
  Unknown10 = 0x10,    // Unknown
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
  Anim = 0x4D494E41,  // Commands for an `Anim` resource.
  Cust = 0x54535543,  // Commands for a `Cust` resource.
  Delt = 0x544C4544,  // Commands for a `Delt` resource.
  End = 0x00444E45,   // Marks the end of the `Film`.
  Pltt = 0x54544C50,  // Commands for a `Pltt` resource.
  View = 0x57454956,  // Commands for the layout of the `Film`.
  Voic = 0x43494F56,  // Commands for `Blas` and `Voic` resources.
};

struct TimeChunk {
  U16 p1;
};

struct MoveChunk {
  U16 p1;
};

struct SpeedChunk {
  U16 p1;
};

struct LayerChunk {
  U16 layer;
  U16 p1;
};

struct AnimationChunk {
  U16 p1;
  U16 p2;
  U16 p3;
};

struct ShiftChunk {
  U16 p1;
  U16 p2;
  U16 p3;
  U16 p4;
  U16 p5;
};

struct Unknown10Chunk {
  U16 p1;
  U16 p2;
  U16 p3;
  U16 p4;
  U16 p5;
  U16 p6;
  U16 p7;
};

struct TransitionChunk {
  U16 p1;
  U16 p2;
  U16 p3;
  U16 p4;
  U16 p5;
  U16 p6;
  U16 p7;
  U16 p8;
};

struct Chunk {
  OpCode opCode;

  union {
    TimeChunk time;
    MoveChunk move;
    SpeedChunk speed;
    LayerChunk layer;
    AnimationChunk animation;
    ShiftChunk shift;
    Unknown10Chunk unknown10;
    TransitionChunk transition;
  };
};

struct Block {
  BlockType type;
  U16 typeIndex;
  nu::DynamicString name;
  nu::DynamicArray<Chunk> chunks;
};

struct Film {
  U32 numberOfFrames;
  nu::DynamicArray<Block> blocks;
};

nu::ScopedPtr<Film> read(nu::InputStream* stream);
void write(nu::OutputStream* stream, const Film& film);

}  // namespace film

#endif  // RESOURCE_FILM_H_
