#pragma once

#include <renderer/sprite_renderer.h>

#include "engine/resources.h"

namespace engine {

class Stage {
public:
  virtual ~Stage();

  virtual bool onReady() = 0;
  virtual void onUpdate(U32 millis) = 0;
  virtual void onRender() = 0;

protected:
  Resources* resources() const {
    assert(m_resources != nullptr);
    return m_resources;
  }

  renderer::SpriteRenderer* renderer() const {
    assert(m_renderer != nullptr);
    return m_renderer;
  }

private:
  friend class Engine;

  void attachToEngine(Resources* resources, renderer::SpriteRenderer* renderer);
  void detachFromEngine();

  Resources* m_resources;
  renderer::SpriteRenderer* m_renderer;
};

}  // namespace engine
