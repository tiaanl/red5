#include <catch2/catch.hpp>

#include "base/dimensions.h"

TEST_CASE("init to zero") {
  PositionI pi;
  CHECK(pi.left == 0);
  CHECK(pi.top == 0);

  PositionF pf;
  CHECK(pf.left == 0.0f);
  CHECK(pf.top == 0.0f);

  SizeI si;
  CHECK(si.width == 0);
  CHECK(si.height == 0);

  SizeF sf;
  CHECK(sf.width == 0.0f);
  CHECK(sf.height == 0.0f);

  RectI ri;
  CHECK(ri.position.left == 0);
  CHECK(ri.position.top == 0);
  CHECK(ri.size.width == 0);
  CHECK(ri.size.height == 0);

  RectF rf;
  CHECK(rf.position.left == 0.0f);
  CHECK(rf.position.top == 0.0f);
  CHECK(rf.size.width == 0.0f);
  CHECK(rf.size.height == 0.0f);
}

TEST_CASE("conversions") {
  SECTION("position i to f") {
    PositionI i{1, 2};
    PositionF f = i;

    CHECK(f.left == 1.0f);
    CHECK(f.top == 2.0f);
  }

  SECTION("position f to i") {
    PositionF f{10.2f, 11.8f};
    PositionI i = f;

    CHECK(i.left == 10);
    CHECK(i.top == 12);
  }

  SECTION("size i to f") {
    SizeI i{1, 2};
    SizeF f = i;

    CHECK(f.width == 1.0f);
    CHECK(f.height == 2.0f);
  }

  SECTION("size f to i") {
    SizeF f{10.7f, 2.4f};
    SizeI i = f;

    CHECK(i.width == 11);
    CHECK(i.height == 2);
  }

  SECTION("rect i to f") {
    RectI i{1, 2, 3, 4};
    RectF f = i;

    CHECK(f.position.left == 1.0f);
    CHECK(f.position.top == 2.0f);
    CHECK(f.size.width == 3.0f);
    CHECK(f.size.height == 4.0f);
  }

  SECTION("rect f to i") {
    RectF f{1.2f, 2.4f, 3.6f, 4.8f};
    RectI i = f;

    CHECK(i.position.left == 1);
    CHECK(i.position.top == 2);
    CHECK(i.size.width == 4);
    CHECK(i.size.height == 5);
  }
}

TEST_CASE("addition") {
  SECTION("position") {
    auto p = PositionI{1, 2} + PositionI{2, 3};
    CHECK(p.left == 3);
    CHECK(p.top == 5);
  }

  SECTION("size") {
    auto s = SizeI{1, 2} + SizeI{2, 3};
    CHECK(s.width == 3);
    CHECK(s.height == 5);
  }

  SECTION("rect + position") {
    auto r = RectI{1, 2, 3, 4} + PositionI{2, 4};
    CHECK(r.position.left == 3);
    CHECK(r.position.top == 6);
    CHECK(r.size.width == 3);
    CHECK(r.size.height == 4);
  }

  SECTION("rect + size") {
    auto r = RectI{1, 2, 3, 4} + SizeI{2, 4};
    CHECK(r.position.left == 1);
    CHECK(r.position.top == 2);
    CHECK(r.size.width == 5);
    CHECK(r.size.height == 8);
  }
}

TEST_CASE("fit into") {
  SECTION("same aspect ratio") {
    RectI image{10, 10, 320, 200};
    RectI frame{100, 100, 1600, 1000};

    auto result = fitInto(image, frame);

    CHECK(result.position.left == 100);
    CHECK(result.position.top == 100);
    CHECK(result.size.width == 1600);
    CHECK(result.size.height == 1000);
  }

  SECTION("frame width is more") {
    RectI image{10, 10, 320, 200};
    RectI frame{100, 100, 2000, 1000};

    auto result = fitInto(image, frame);

    CHECK(result.position.left == 300);
    CHECK(result.position.top == 100);
    CHECK(result.size.width == 1600);
    CHECK(result.size.height == 1000);
  }

  SECTION("frame height is more") {
    RectI image{10, 10, 320, 200};
    RectI frame{100, 100, 1600, 1400};

    auto result = fitInto(image, frame);

    CHECK(result.position.left == 100);
    CHECK(result.position.top == 300);
    CHECK(result.size.width == 1600);
    CHECK(result.size.height == 1000);
  }
}
