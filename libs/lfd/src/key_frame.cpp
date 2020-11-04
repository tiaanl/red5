#include "lfd/key_frame.h"

namespace lfd {

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

  assert(false);
  return "Unknown";
}

// static
KeyFrame KeyFrame::time(I16 frame) {
  return KeyFrame(OpCode::Time, {frame});
}

// static
KeyFrame KeyFrame::display(bool visible) {
  I16 v = visible ? 1 : 0;
  return KeyFrame(OpCode::Display, {v});
}

}  // namespace lfd
