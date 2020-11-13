#include <engine/engine.h>
#include <game/scene_manager.h>
#include <xwing/controllers/main_menu_scene_controller.h>
#include <xwing/controllers/register_scene_controller.h>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Red 5")) {
    return 1;
  }

  game::SceneManager sceneManager{&engine};
  sceneManager.registerScene("register", {"register"},
                             game::sceneControllerFactory<xwing::RegisterSceneController>());
  sceneManager.registerScene("mainmenu", {"mainmenu"},
                             game::sceneControllerFactory<xwing::MainMenuSceneController>());

  sceneManager.switchToScene("register");

#if 0
  auto gameStageState = game::GameStageState::create(&engine.renderer());

#if 0
  auto controller = std::make_unique<xwing::RegisterSceneController>();
#else
  auto controller = std::make_unique<xwing::MainMenuSceneController>();
#endif

  auto stage = std::make_unique<game::SceneStage>(std::move(gameStageState), std::move(controller));

  engine.setStage(std::move(stage));
#endif

  return engine.run() ? 0 : 1;
}
