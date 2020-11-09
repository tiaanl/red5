#pragma once

namespace engine {

class Stage;

class EngineOps {
public:
  void switchStage(std::unique_ptr<Stage> stage);

private:
  friend class Engine;

  std::unique_ptr<Stage> m_switchToStage;
};

}  // namespace engine
