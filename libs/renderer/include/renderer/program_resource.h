#pragma once

#include "renderer/resource_container.h"

namespace renderer {

struct ProgramData {
  U32 id;
};

class ProgramContainer : public ResourceContainer<ProgramData> {
public:
  ~ProgramContainer();

  Identifier create(const char* vertexShaderSource, const char* fragmentShaderSource);
  void destroy(Identifier id);
};

}  // namespace
