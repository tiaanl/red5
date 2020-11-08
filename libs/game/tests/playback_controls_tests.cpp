#include <catch2/catch.hpp>

#include "game/playback_controls.h"

namespace game {

TEST_CASE("minimum frame count is 1") {
  PlaybackControls pc{1, 0};
  CHECK(pc.firstFrame() == 1);
  CHECK(pc.frameCount() == 1);
  CHECK(pc.currentFrame() == 1);
  pc.nextFrame();
  CHECK(pc.currentFrame() == 1);
}

TEST_CASE("does nothing if paused") {
  PlaybackControls pc{0, 1};
  CHECK(pc.currentFrame() == 0);
  pc.pause();
  pc.nextFrame();
  CHECK(pc.currentFrame() == 0);
}

TEST_CASE("looping") {
  PlaybackControls pc{0, 3};

  SECTION("loop forward") {
    pc.play(PlayDirection::Forward, LoopMode::Loop);
    CHECK(pc.currentFrame() == 0);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 1);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 2);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 0);
  }

  SECTION("loop backward") {
    pc.play(PlayDirection::Backward, LoopMode::Loop);
    pc.jumpToFrame(2);
    CHECK(pc.currentFrame() == 2);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 1);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 0);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 2);
  }

  SECTION("stop forward") {
    pc.play(PlayDirection::Forward, LoopMode::Stop);
    CHECK(pc.currentFrame() == 0);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 1);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 2);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 2);
  }

  SECTION("stop backward") {
    pc.play(PlayDirection::Backward, LoopMode::Stop);
    pc.jumpToFrame(1);
    CHECK(pc.currentFrame() == 1);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 0);
    pc.nextFrame();
    CHECK(pc.currentFrame() == 0);
  }
}

}  // namespace game
