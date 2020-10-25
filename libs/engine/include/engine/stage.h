#pragma once

#include <renderer/renderer.h>

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

  renderer::Renderer* renderer() const {
    assert(m_renderer != nullptr);
    return m_renderer;
  }

private:
  friend class Engine;

  void attachToEngine(Resources* resources, renderer::Renderer* renderer);
  void detachFromEngine();

  Resources* m_resources;
  renderer::Renderer* m_renderer;
};

}  // namespace engine
