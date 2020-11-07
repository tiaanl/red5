#pragma once

namespace engine {

struct Color {
  U8 red;
  U8 green;
  U8 blue;
  U8 alpha;
};

struct RGB {
  U8 red;
  U8 green;
  U8 blue;
};

struct ColorF {
  F32 red;
  F32 green;
  F32 blue;
  F32 alpha;
};

}  // namespace engine
