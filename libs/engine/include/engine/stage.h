#pragma once

#include <renderer/sprite_renderer.h>

namespace engine {

class Stage {
public:
  virtual ~Stage();

  virtual bool onLoad() = 0;

  virtual void onStageResized(I32 width, I32 height);

  virtual void onMouseMoved(I32 x, I32 y);

  virtual void onUpdate(U32 millis) = 0;

  virtual void onRender() = 0;

protected:
  renderer::Renderer* m_renderer;

  virtual bool attachToEngine(renderer::Renderer* renderer);
  virtual void detachFromEngine();

private:
  friend class Engine;
};

}  // namespace engine
