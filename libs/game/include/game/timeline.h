#pragma once

#include <lfd/film.h>

#include "playback_controls.h"

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
  void build(U32 frameCount, const std::vector<lfd::KeyFrame>& keyFrames);

  Frame& frame(U32 frame) {
    assert(frame < m_frames.size());
    return m_frames[frame];
  }

  const Frame& frame(U32 frame) const {
    assert(frame < m_frames.size());
    return m_frames[frame];
  }

private:
  std::vector<Frame> m_frames;
};

}  // namespace game
