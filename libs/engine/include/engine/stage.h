#pragma once

namespace engine {

class Renderer;

class Stage {
public:
  virtual ~Stage();

  virtual bool onLoad() = 0;
  virtual void onStageResized(I32 width, I32 height);
  virtual void onMouseMoved(const PositionI& mousePosition);
  virtual void onUpdate(U32 millis) = 0;
  virtual void onRender() = 0;

protected:
  Renderer* m_renderer;

  virtual bool attachToEngine(Renderer* renderer);
  virtual void detachFromEngine();

private:
  friend class Engine;
};

}  // namespace engine
