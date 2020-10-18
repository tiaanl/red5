#pragma once

#include "lfd/image.h"

class Animation : public Resource {
public:
  ~Animation() override;

  const std::vector<Image>& frames() const {
    return m_frames;
  }

  void read(base::InputStream* stream, MemSize size) override;
  void write(base::OutputStream* stream) override;

private:
  std::vector<Image> m_frames;
};
