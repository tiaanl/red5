#include <cmath>

RectI fitInto(const RectI& source, const RectI& destination) {
  F32 widthScale = static_cast<F32>(destination.size.width) / static_cast<F32>(source.size.width);
  F32 heightScale =
      static_cast<F32>(destination.size.height) / static_cast<F32>(source.size.height);

  I32 newWidth = destination.size.width;
  I32 newHeight = static_cast<I32>(std::round(static_cast<F32>(source.size.height) * widthScale));
  if (heightScale < widthScale) {
    newWidth = static_cast<I32>(std::round(static_cast<F32>(source.size.width) * heightScale));
    newHeight = destination.size.height;
  }

  I32 newLeft = static_cast<I32>(std::round(
      static_cast<F32>(destination.position.left) +
      (static_cast<F32>(destination.size.width) / 2.0f - static_cast<F32>(newWidth) / 2.0f)));

  I32 newTop = static_cast<I32>(std::round(
      static_cast<F32>(destination.position.top) +
      (static_cast<F32>(destination.size.height) / 2.0f - static_cast<F32>(newHeight) / 2.0f)));

  RectI result{newLeft, newTop, newWidth, newHeight};

#if 0
  spdlog::info("fitInto (({}, {}), ({}, {})) into (({}, {}), ({}, {})) = (({}, {}), ({}, {}))",
               source.position.left, source.position.top, source.size.width, source.size.height,
               destination.position.left, destination.position.top, destination.size.width,
               destination.size.height, result.position.left, result.position.top,
               result.size.width, result.size.height);
#endif  // 0

  return result;
}
