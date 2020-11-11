#include <engine/engine.h>
#include <game/scene_stage.h>
#include <xwing/controllers/main_menu_scene_controller.h>
#include <xwing/controllers/register_scene_controller.h>

#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Red 5")) {
    return 1;
  }

  auto gameStageState = game::GameStageState::create(&engine.renderer());

#if 0
  auto controller = std::make_unique<xwing::RegisterSceneController>();
#else
  auto controller = std::make_unique<xwing::MainMenuSceneController>();
#endif

  auto stage = std::make_unique<game::SceneStage>(std::move(gameStageState), std::move(controller));

  engine.setStage(std::move(stage));

  return engine.run() ? 0 : 1;
}
