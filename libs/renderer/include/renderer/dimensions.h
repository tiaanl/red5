#pragma once

namespace renderer {

struct Position {
  I32 left;
  I32 top;

  Position& operator+=(const Position& other) {
    left += other.left;
    top += other.top;

    return *this;
  }
};

struct Size {
  I32 width;
  I32 height;
};

struct Rect {
  Position position;
  Size size;

  Rect(I32 left, I32 top, I32 width, I32 height) : position{left, top}, size{width, height} {}
  Rect(const Position& position, const Size& size) : position{position}, size{size} {}
};

}  // namespace renderer
