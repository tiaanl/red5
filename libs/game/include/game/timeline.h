#pragma once

#include <lfd/film.h>

namespace game {

struct Frame {
  bool visible = false;
  I16 layer = 0;
  PositionI offset = {0, 0};
  I16 spriteIndex = 0;
  RectI window = {0, 0, 0, 0};
  I16 orientation = 0;
};

class Timeline {
public:
  Timeline();

  void build(U32 frameCount, const std::vector<lfd::KeyFrame>& keyFrames);

  U32 currentFrameIndex() const {
    return m_currentFrameIndex;
  }

  void setCurrentFrame(U32 frame);
  void nextFrame();

  const Frame& currentFrame() const {
    return frame(m_currentFrameIndex);
  }

  const Frame& frame(U32 frame) const {
    assert(frame < m_frames.size());
    return m_frames[frame];
  }

private:
  U32 m_currentFrameIndex;
  std::vector<Frame> m_frames;
};

}  // namespace game
