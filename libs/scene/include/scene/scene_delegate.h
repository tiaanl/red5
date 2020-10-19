#pragma once

namespace scene {

class Delegate {
public:
  virtual ~Delegate();

  virtual void onEvent(I16 event);
};

}  // namespace scene
