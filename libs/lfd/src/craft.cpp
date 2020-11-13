#include "lfd/craft.h"

#include "base/streams/input_stream.h"

namespace lfd {

Craft::~Craft() = default;

void Craft::read(base::InputStream* stream, MemSize size) {
  auto totalDataSize = stream->readU16();
  auto componentCount = stream->readU8();
  auto shadingCount = stream->readU8();
  spdlog::info("Craft :: totalDataSize: {}, componentCount: {}, shadingCount: {}", totalDataSize,
               componentCount, shadingCount);

  assert(shadingCount == 0);
#if 0
  for (auto i = 0; i < shadingCount; ++i) {
    stream->advance(16);
  }
#endif  // 0

  // auto componentJumpStart = stream->getPosition();

  auto componentJumps = new I16[componentCount];

  for (auto i = 0; i < componentCount; ++i) {
    componentJumps[i] = stream->readI16();
    spdlog::info("  componentJump ({}): {}", i, componentJumps[i]);
  }

  for (auto c = 0; c < componentCount; ++c) {
    // Pos should be 14

    I16 lodCount = 0;
    for (;;) {
      auto distance = stream->readI32();
      if (distance == std::numeric_limits<I32>::max()) {
        spdlog::info("    distance: {}", distance);
        break;
      }
      auto jump = stream->readI16();

      ++lodCount;

      spdlog::info("    distance: {}, jump: {}", distance, jump);
    }

    stream->readI16();  // What is this?

    // spdlog::info("position: {}", stream->getPosition());

    spdlog::info("    signature?: {}", stream->readU8());
    spdlog::info("    unknown1?: {}", stream->readU8());
    U8 vertexCount = stream->readU8();
    spdlog::info("    vertexCount?: {}", vertexCount);
    spdlog::info("    unknown2?: {}", stream->readU8());
    U8 shapeCount = stream->readU8();
    spdlog::info("    shapeCount?: {}", shapeCount);

    for (auto colorIndex = 0; colorIndex < shapeCount; ++colorIndex) {
      spdlog::info("    colorIndex: {}", stream->readU8());
    }

    spdlog::info("      x: {}, y: {}, z: {}", stream->readI16(), stream->readI16(),
                 stream->readI16());
    spdlog::info("      x: {}, y: {}, z: {}", stream->readI16(), stream->readI16(),
                 stream->readI16());

    struct V3 {
      I16 v[3];

      void read(base::InputStream* stream) {
        for (I16& i : v) {
          i = stream->readI16();
        }
      }
    };

    std::vector<V3> vertices{vertexCount};
    for (auto v = 0; v < vertexCount; ++v) {
      vertices[v].read(stream);

      for (auto vv = 0; vv < 3; vv++) {
        if ((vertices[v].v[vv] & 0xFF00) == 0x7F00) {
          int delta = (vertices[v].v[vv] & 0x00FF) >> 1;
          vertices[v].v[vv] = vertices[v - delta].v[vv];
        }
      }

      spdlog::info("      x: {}, y: {}, z: {}", vertices[v].v[0], vertices[v].v[1],
                   vertices[v].v[2]);
    }

    for (auto s = 0; s < shapeCount; ++s) {
      V3 normal;
      normal.read(stream);
      auto shapeSize = stream->readI16();

      spdlog::info("    normal: [{}, {}, {}], shapeSize: {}", normal.v[0], normal.v[1], normal.v[2],
                   shapeSize);
    }

    break;
  }
}

void Craft::write(base::OutputStream* stream) {}

}  // namespace lfd
