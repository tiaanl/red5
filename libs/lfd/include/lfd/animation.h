#pragma once

#include "lfd/image.h"

class Animation : public Resource {
public:
  ~Animation() override;

  const std::vector<Image>& frames() const {
    return m_frames;
  }

  void read(nu::InputStream* stream, MemSize size) override;
  void write(nu::OutputStream* stream) override;

private:
  std::vector<Image> m_frames;
};
