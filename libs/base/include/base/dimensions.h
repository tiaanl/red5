#pragma once

template <typename T>
struct Position {
  T left;
  T top;

  friend Position operator+(const Position& left, const Position& right) {
    return {left.left + right.left, left.top + right.top};
  }

  Position& operator+=(const Position& other) {
    left += other.left;
    top += other.top;

    return *this;
  }
};

using PositionI = Position<I32>;
using PositionF = Position<F32>;

template <typename T>
struct Size {
  T width;
  T height;
};

using SizeI = Size<I32>;
using SizeF = Size<F32>;

template <typename T>
struct Rect {
  Position<T> position;
  Size<T> size;

  Rect(I32 left, I32 top, I32 width, I32 height) : position{left, top}, size{width, height} {}
  Rect(const Position<T>& position, const Size<T>& size) : position{position}, size{size} {}

  bool contains(const Position<T>& p) const {
    return (p.left >= position.left && p.left < position.left + size.width) &&
           (p.top >= position.top && p.top < position.top + size.height);
  }

  friend Rect operator+(const Rect& left, const Position<T>& right) {
    return {left.position + right, left.size};
  }
};

using RectI = Rect<I32>;
using RectF = Rect<F32>;

RectI fitInto(const RectI& source, const RectI& destination);
