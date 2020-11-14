#include <engine/engine.h>
#include <game/scene_builder.h>
#include <game/scene_manager.h>
#include <xwing/controllers/main_menu_scene_controller.h>
#include <xwing/controllers/register_scene_controller.h>

namespace fs = std::filesystem;

game::SceneBuilder scene(std::string_view filmName) {
  return game::SceneBuilder(filmName);
}

template <typename SceneControllerType>
game::SceneDescription interactive(std::string_view resourceFile, std::string_view filmName) {
  return game::SceneBuilder{filmName}
      .resourceFile(resourceFile)
      .autoPlay(false)
      .template sceneController<SceneControllerType>()
      .build();
}

game::SceneDescription cutscene(std::string_view resourceFile, std::string_view filmName,
                                std::string_view nextScene) {
  return game::SceneBuilder{filmName}.resourceFile(resourceFile).autoPlay(true, nextScene).build();
}

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Red 5")) {
    return 1;
  }

  game::SceneManager sceneManager{R"(C:\XWING\RESOURCE)", &engine};

  sceneManager.registerScene("logo", cutscene("LOGO", "logo_f", "register"));

  sceneManager.registerScene("register",
                             interactive<xwing::RegisterSceneController>("REGISTER", "pilot"));
  sceneManager.registerScene("mainmenu",
                             interactive<xwing::MainMenuSceneController>("MAINMENU", "mainmenu"));

  sceneManager.switchToScene("logo");

  return engine.run() ? 0 : 1;
}
