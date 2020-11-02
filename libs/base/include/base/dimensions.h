#pragma once

struct Position {
  I32 left;
  I32 top;

  friend Position operator+(const Position& left, const Position& right) {
    return {left.left + right.left, left.top + right.top};
  }

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

  friend Rect operator+(const Rect& left, const Position& right) {
    return {left.position + right, left.size};
  }
};

Rect fitInto(const Rect& source, const Rect& destination);
