#pragma once

#include "resource_container.h"

namespace engine {

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
