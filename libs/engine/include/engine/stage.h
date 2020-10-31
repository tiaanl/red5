#pragma once

#include <renderer/sprite_renderer.h>

namespace engine {

class Stage {
public:
  virtual ~Stage();

  virtual bool onLoad() = 0;
  virtual void onUpdate(U32 millis) = 0;
  virtual void onRender() = 0;

protected:
  renderer::Renderer* m_renderer;

  virtual void attachToEngine(renderer::Renderer* renderer);
  virtual void detachFromEngine();

private:
  friend class Engine;
};

}  // namespace engine
