#include "game/playback_controls.h"

namespace game {

namespace {

I32 direction(PlayDirection playDirection) {
  switch (playDirection) {
    case PlayDirection::Forward:
      return 1;

    case PlayDirection::Backward:
      return -1;
  }

  return 0;
}

}  // namespace

PlaybackControls::PlaybackControls()
  : m_firstFrame{0},
    m_frameCount{1},
    m_currentFrame{0},
    m_direction{0},
    m_loopMode{LoopMode::Stop} {}

PlaybackControls::PlaybackControls(I32 firstFrame, I32 frameCount)
  : m_firstFrame{firstFrame},
    m_frameCount{std::max<I32>(frameCount, 1)},
    m_currentFrame{firstFrame},
    m_direction{0},
    m_loopMode{LoopMode::Stop} {}

void PlaybackControls::setFrameRange(I32 firstFrame, I32 frameCount) {
  m_firstFrame = firstFrame;
  m_frameCount = std::max(1, frameCount);
  jumpToFrame(m_currentFrame);
}

void PlaybackControls::jumpToFrame(I32 frame) {
  m_currentFrame = std::min<I32>(m_firstFrame + m_frameCount, std::max<I32>(m_firstFrame, frame));
}

void PlaybackControls::jumpToRandomFrame() {
  jumpToFrame(std::rand() % m_frameCount + m_firstFrame);
}

void PlaybackControls::play(PlayDirection playDirection, LoopMode loopMode,
                            std::optional<I32> stopFrame) {
  m_direction = direction(playDirection);
  m_stopFrame = stopFrame;
  m_loopMode = loopMode;
}

void PlaybackControls::pause() {
  m_direction = 0;
  m_stopFrame.reset();
  m_loopMode = LoopMode::Stop;
}

void PlaybackControls::nextFrame() {
  if (m_direction < 0 && m_currentFrame <= m_firstFrame) {
    switch (m_loopMode) {
      case LoopMode::Loop:
        m_currentFrame = m_firstFrame + m_frameCount - 1;
        break;

      case LoopMode::PingPong:
        m_direction *= -1;
        break;

      case LoopMode::Stop:
        pause();
        break;
    }
  } else if (m_direction > 0 && m_currentFrame >= m_firstFrame + m_frameCount - 1) {
    switch (m_loopMode) {
      case LoopMode::Loop:
        m_currentFrame = m_firstFrame;
        break;

      case LoopMode::PingPong:
        m_direction *= -1;
        break;

      case LoopMode::Stop:
        pause();
        break;
    }
  } else {
    m_currentFrame += m_direction;
  }

  if (m_stopFrame.has_value() && m_currentFrame == m_stopFrame.value()) {
    pause();
  }
}

}  // namespace game
