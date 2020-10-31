#pragma once

#include <renderer/sprite_renderer.h>

namespace engine {

class Stage {
public:
  virtual ~Stage();

  virtual bool onReady() = 0;
  virtual void onUpdate(U32 millis) = 0;
  virtual void onRender() = 0;

protected:
  renderer::SpriteRenderer* renderer() const {
    assert(m_renderer != nullptr);
    return m_renderer;
  }

private:
  friend class Engine;

  void attachToEngine(renderer::SpriteRenderer* renderer);
  void detachFromEngine();

  renderer::SpriteRenderer* m_renderer;
};

}  // namespace engine
