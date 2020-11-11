#pragma once

#include "engine/engine_ops.h"

namespace engine {

class Renderer;

class Stage {
public:
  Stage() = default;

  Stage(const Stage&) = delete;
  Stage& operator=(const Stage&) = delete;

  virtual ~Stage();

  virtual bool onAttachedToEngine(Renderer* renderer) = 0;

  virtual void onStageResized(I32 width, I32 height);

  virtual void onMouseMoved(const PositionI& mousePosition);
  virtual void onMousePressed(const PositionI& mousePosition, U8 buttons);
  virtual void onMouseReleased(const PositionI& mousePosition, U8 buttons);

  virtual void onUpdate(U32 millis) = 0;

  virtual void onRender() = 0;

protected:
  Renderer& renderer() {
    assert(m_renderer);
    return *m_renderer;
  }

  EngineOps& engineOps() {
    assert(m_engineOps);
    return *m_engineOps;
  }

private:
  friend class Engine;

  bool attachToEngine(EngineOps* engineOps, Renderer* renderer);
  void detachFromEngine();

  EngineOps* m_engineOps = nullptr;
  Renderer* m_renderer = nullptr;
};

}  // namespace engine
