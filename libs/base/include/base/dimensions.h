#pragma once

template <typename To, typename From>
To convert(From from);

template <>
inline I32 convert(I32 from) {
  return from;
}

template <>
inline I32 convert(F32 from) {
  return static_cast<I32>(std::round(from));
}

template <>
inline I32 convert(U16 from) {
  return static_cast<I32>(from);
}

template <>
inline F32 convert(F32 from) {
  return from;
}

template <>
inline constexpr F32 convert(I32 from) {
  return static_cast<F32>(from);
}

template <typename T>
struct Position {
  T left;
  T top;

  Position() : left{convert<T, I32>(0)}, top{convert<T, I32>(0)} {}

  template <typename U>
  Position(U left, U top) : left{convert<T>(left)}, top{convert<T>(top)} {}

  template <typename U>
  Position(const Position<U>& other) : left{convert<T>(other.left)}, top{convert<T>(other.top)} {}

  friend Position operator+(const Position& a, const Position& b) {
    return {a.left + b.left, a.top + b.top};
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

  Size() : width{convert<T, I32>(0)}, height{convert<T, I32>(0)} {}

  template <typename U>
  Size(const Size<U>& other) : width{convert<T>(other.width)}, height{convert<T>(other.height)} {}

  template <typename U>
  Size(U width, U height) : width{convert<T>(width)}, height{convert<T>(height)} {}

  friend Size operator+(const Size& a, const Size& b) {
    return {a.width + b.width, a.height + b.height};
  }
};

using SizeI = Size<I32>;
using SizeF = Size<F32>;

template <typename T>
struct Rect {
  Position<T> position;
  Size<T> size;

  Rect() : position{}, size{} {}

  Rect(T left, T top, T width, T height) : position{left, top}, size{width, height} {}

  Rect(const Position<T>& position, const Size<T>& size) : position{position}, size{size} {}

  template <typename U>
  Rect(const Rect<U>& other)
    : Rect{convert<T>(other.position.left), convert<T>(other.position.top),
           convert<T>(other.size.width), convert<T>(other.size.height)} {}

  bool contains(const Position<T>& p) const {
    return (p.left >= position.left && p.left < position.left + size.width) &&
           (p.top >= position.top && p.top < position.top + size.height);
  }

  friend Rect operator+(const Rect& left, const Position<T>& right) {
    return {left.position + right, left.size};
  }

  friend Rect operator+(const Rect& left, const Size<T>& right) {
    return {left.position, left.size + right};
  }
};

using RectI = Rect<I32>;
using RectF = Rect<F32>;

RectI fitInto(const RectI& source, const RectI& destination);
