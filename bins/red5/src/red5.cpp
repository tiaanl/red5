#include <engine/engine.h>
#include <game/scene_builder.h>
#include <game/scene_manager.h>
#include <xwing/controllers/main_menu_scene_controller.h>
#include <xwing/controllers/register_scene_controller.h>

namespace fs = std::filesystem;

game::SceneBuilder scene(std::string_view filmName) {
  return game::SceneBuilder(filmName);
}

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Red 5")) {
    return 1;
  }

  game::SceneManager sceneManager{R"(C:\XWING\RESOURCE)", &engine};

  sceneManager.registerScene("register", scene("pilot")
                                             .resourceFile("REGISTER")
                                             .sceneController<xwing::RegisterSceneController>()
                                             .build());

  sceneManager.registerScene("mainmenu", scene("mainmenu")
                                             .resourceFile("MAINMENU")
                                             .sceneController<xwing::MainMenuSceneController>()
                                             .build());

  sceneManager.switchToScene("register");

  return engine.run() ? 0 : 1;
}
