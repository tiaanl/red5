#include "game/timeline.h"

namespace game {

namespace {

struct BuildContext {
  PositionI offsetPerFrame = {0, 0};
  I32 animationDirection = 0;
};

#if 0
U32 nextTimeKeyFrame(const std::vector<lfd::KeyFrame>& keyFrames, U32 current) {
  while (current < keyFrames.size()) {
    auto opCode = keyFrames[current].opCode;
    if (opCode == lfd::OpCode::Time || opCode == lfd::OpCode::End) {
      break;
    }
    ++current;
  }

  return current;
}
#endif  // 0

void applyKeyFrame(Frame* frame, BuildContext* buildContext, const lfd::KeyFrame& keyFrame) {
  switch (keyFrame.opCode) {
    case lfd::OpCode::Move: {
      //      assert(keyFrame.variables.size() == 4);
      frame->offset.left = keyFrame.variables[0];
      frame->offset.top = keyFrame.variables[1];
      //      assert(keyFrame.variables[2] == 0);
      //      assert(keyFrame.variables[3] == 0);
      break;
    }

    case lfd::OpCode::Speed: {
      //      assert(keyFrame.variables.size() == 4);
      buildContext->offsetPerFrame.left = keyFrame.variables[0];
      buildContext->offsetPerFrame.top = keyFrame.variables[1];
      //      assert(keyFrame.variables[2] == 0);
      //      assert(keyFrame.variables[3] == 0);
      break;
    }

    case lfd::OpCode::Layer: {
      assert(keyFrame.variables.size() == 1);
      frame->layer = keyFrame.variables[0];
      break;
    }

    case lfd::OpCode::Frame: {
      // assert(keyFrame.variables.size() == 2);
      frame->spriteIndex = keyFrame.variables[0];
      assert(keyFrame.variables[1] == 0);
      break;
    }

    case lfd::OpCode::Animation: {
      //      assert(keyFrame.variables.size() == 2);
      buildContext->animationDirection = keyFrame.variables[0];
      //      assert(keyFrame.variables[1] == 0);
      break;
    }

    case lfd::OpCode::Event: {
      // We skip events in timeline building.
      break;
    }

    case lfd::OpCode::Region: {
      // TODO: Implement
      break;
    }

    case lfd::OpCode::Window: {
      assert(keyFrame.variables.size() == 4);
      frame->window.position.left = keyFrame.variables[0];
      frame->window.position.top = keyFrame.variables[1];
      frame->window.size.width = keyFrame.variables[2];
      frame->window.size.height = keyFrame.variables[3];
      break;
    }

    case lfd::OpCode::Shift: {
      assert(keyFrame.variables.size() == 4);
      frame->offset.left = keyFrame.variables[0];
      frame->offset.top = keyFrame.variables[1];
      assert(keyFrame.variables[2] == 0);
      assert(keyFrame.variables[3] == 0);
      break;
    }

    case lfd::OpCode::Display: {
      assert(keyFrame.variables.size() == 1);
      frame->visible = keyFrame.variables[0] != 0;
      break;
    }

    case lfd::OpCode::Orientation: {
      assert(keyFrame.variables.size() == 2);
      frame->orientation = keyFrame.variables[0];
      // assert(keyFrame.variables[1] == 0);
      break;
    }

    default: {
      spdlog::error("OpCode not handled: {}", lfd::opCodeToString(keyFrame.opCode));
      assert(false);
      break;
    }
  }
}

}  // namespace

void Timeline::build(U32 frameCount, const std::vector<lfd::KeyFrame>& keyFrames) {
  // There will always be at least one frame.
  m_frames.resize(std::max<U32>(1, frameCount));

  if (keyFrames.empty()) {
    return;
  }

  // The first key frame should always be 0?
#if 0
  U32 keyFrameIndex = nextTimeKeyFrame(keyFrames, 0);
  if (keyFrameIndex >= keyFrames.size()) {
    // None found.
    return;
  }
#else
  U32 keyFrameIndex = 0;
#endif  // 0

  assert(keyFrames[keyFrameIndex].opCode == lfd::OpCode::Time ||
         keyFrames[keyFrameIndex].opCode == lfd::OpCode::End);

  BuildContext buildContext;
  Frame currentFrame;

  for (U32 frameIndex = 0; frameIndex < frameCount; ++frameIndex) {
    const auto& keyFrame = keyFrames[keyFrameIndex];

    if (keyFrame.opCode == lfd::OpCode::Time &&
        static_cast<U32>(keyFrame.variables[0]) == frameIndex) {
      ++keyFrameIndex;
      for (; keyFrameIndex < keyFrames.size(); ++keyFrameIndex) {
        if (keyFrames[keyFrameIndex].opCode == lfd::OpCode::Time ||
            keyFrames[keyFrameIndex].opCode == lfd::OpCode::End) {
          break;
        }
        applyKeyFrame(&currentFrame, &buildContext, keyFrames[keyFrameIndex]);
      }
    }

    // Updates for each frame:

    currentFrame.offset += buildContext.offsetPerFrame;
    currentFrame.spriteIndex += buildContext.animationDirection;

    m_frames[frameIndex] = currentFrame;
  }
}

}  // namespace game
