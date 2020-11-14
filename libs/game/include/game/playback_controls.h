#pragma once

#include <optional>

namespace game {

enum class PlayDirection {
  Forward,
  Backward,
};

enum class LoopMode {
  Loop,
  PingPong,
  Stop,
};

class PlaybackControls {
public:
  PlaybackControls();
  PlaybackControls(I32 firstFrame, I32 frameCount);

  I32 firstFrame() const {
    return m_firstFrame;
  }

  I32 frameCount() const {
    return m_frameCount;
  }

  void setFrameRange(I32 firstFrame, I32 frameCount);

  bool isPlaying() const {
    return m_direction != 0;
  }

  I32 currentFrame() const {
    return m_currentFrame;
  }

  void jumpToFrame(I32 frame);
  void jumpToRandomFrame();

  void play(PlayDirection playDirection, LoopMode loopMode, std::optional<I32> stopFrame = {});
  void pause();

  void nextFrame();

private:
  I32 m_firstFrame;
  I32 m_frameCount;

  I32 m_currentFrame;

  I32 m_direction;
  std::optional<I32> m_stopFrame;
  LoopMode m_loopMode;
};

}  // namespace game
