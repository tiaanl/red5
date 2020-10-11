#ifndef RESOURCE_FILM_H_
#define RESOURCE_FILM_H_

#include <stdint.h>

#include "lfd.h"

namespace film {

enum class OpCode : uint16_t {
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
  Unknown = 0x17,      // Unknown.
  Preload = 0x18,      // Possibly defines file the sound is located in.
  Sound = 0x19,        // Determines playback controls.
  Stereo = 0x1C,       // Determines stereo playback controls.
};

enum class BlockType : uint32_t {
  Undefined,
  Anim = 0x4D494E41,  // Commands for an `Anim` resource.
  Cust = 0x54535543,  // Commands for a `Cust` resource.
  Delt = 0x544C4544,  // Commands for a `Delt` resource.
  End = 0x00444E45,   // Marks the end of the `Film`.
  Pltt = 0x54544C50,  // Commands for a `Pltt` resource.
  View = 0x57454956,  // Commands for the layout of the `Film`.
  Voic = 0x43494F56,  // Commands for `Blas` and `Voic` resources.
};

struct Chunk {
  uint16_t length;
};

struct Block {
  char type[4];
  char name[8];
  uint32_t length;
  uint16_t type_index;
  uint16_t number_of_chunks;
  uint16_t chunk_data_length;
};

struct Header {
  uint16_t reserved;
  uint16_t number_of_frames;
  uint16_t number_of_blocks;
};

bool load(Data data);

}  // namespace film

#endif  // RESOURCE_FILM_H_
