#pragma once

#include <string>
#include <vector>

#include "lfd/key_frame.h"
#include "lfd/resource.h"
#include "lfd/resource_type.h"

namespace base {
class InputStream;
class OutputStream;
}  // namespace base

namespace lfd {

enum class BlockType : U32 {
  Anim = static_cast<U32>(ResourceType::Animation),
  Cust = 0x54535543,
  Delt = static_cast<U32>(ResourceType::Image),
  Pltt = static_cast<U32>(ResourceType::Palette),
  View = 0x57454956,
  Voic = static_cast<U32>(ResourceType::Voice),

  // Marks the end of the `Film`.
  End = 0x00444E45,
};

class Film : public Resource {
public:
  struct Block {
    BlockType type;
    U16 typeIndex;
    std::string name;
    std::vector<KeyFrame> keyFrames;
  };

  ~Film() override;

  U32 frameCount() const {
    return m_frameCount;
  }

  const std::vector<Block>& blocks() const {
    return m_blocks;
  }

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  U32 m_frameCount;
  std::vector<Block> m_blocks;
};

const char* blockTypeToString(BlockType blockType);

}  // namespace lfd
