#include "game/timeline.h"

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

namespace game {

TEST_CASE("build timeline with no keyframes") {
  Timeline timeline;

  std::vector<lfd::KeyFrame> keyFrames = {};

  timeline.build(1, keyFrames);
}

TEST_CASE("can set visibility") {
  Timeline timeline;

  timeline.build(2, {
                        lfd::KeyFrame::time(0),         //
                        lfd::KeyFrame::display(true),   //
                        lfd::KeyFrame::time(1),         //
                        lfd::KeyFrame::display(false),  //
                    });

  CHECK(timeline.frame(0).visible == true);
  CHECK(timeline.frame(1).visible == false);
}

TEST_CASE("can set visibility with gaps") {
  Timeline timeline;

  timeline.build(3, {
                        lfd::KeyFrame::time(0),         //
                        lfd::KeyFrame::display(true),   //
                        lfd::KeyFrame::time(2),         //
                        lfd::KeyFrame::display(false),  //
                    });

  CHECK(timeline.frame(0).visible == true);
  CHECK(timeline.frame(1).visible == true);
  CHECK(timeline.frame(2).visible == false);
}

}  // namespace game
