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

  virtual bool onLoad() = 0;

  virtual void onStageResized(I32 width, I32 height);

  virtual void onMouseMoved(const PositionI& mousePosition);
  virtual void onMousePressed(const PositionI& mousePosition, U8 buttons);
  virtual void onMouseReleased(const PositionI& mousePosition, U8 buttons);

  virtual void onUpdate(U32 millis) = 0;

  virtual void onRender() = 0;

protected:
  EngineOps* m_engineOps = nullptr;
  Renderer* m_renderer = nullptr;

  virtual bool attachToEngine(EngineOps* engineOps, Renderer* renderer);
  virtual void detachFromEngine();

private:
  friend class Engine;
};

}  // namespace engine
