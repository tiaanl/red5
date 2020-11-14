#include <engine/engine.h>
#include <game/scene_builder.h>
#include <game/scene_manager.h>
#include <xwing/controllers/main_menu_scene_controller.h>
#include <xwing/controllers/register_scene_controller.h>

namespace fs = std::filesystem;

template <typename SceneControllerType>
game::SceneDescription interactive(std::string_view resourceFile, std::string_view filmName) {
  return game::SceneBuilder{filmName}
      .resourceFile(resourceFile)
      .autoPlay(false)
      .template sceneController<SceneControllerType>()
      .build();
}

game::SceneDescription movie(std::string_view resourceFile, std::string_view filmName,
                             std::string_view nextScene) {
  return game::SceneBuilder{filmName}.resourceFile(resourceFile).autoPlay(true, nextScene).build();
}

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Red 5")) {
    return 1;
  }

  game::SceneManager sceneManager{R"(C:\XWING\RESOURCE)", &engine};

  // Intro:
  sceneManager.registerScene("intro_1", movie("LOGO", "logo_f", "intro_3"));
  // sceneManager.registerScene("intro_2", movie("TITLE", "noname", "intro_3"));
  sceneManager.registerScene("intro_3", movie("INTRO", "intro1_f", "intro_4"));
  sceneManager.registerScene("intro_4", movie("BRIDGE1", "brdg1_f", "intro_5"));
  sceneManager.registerScene("intro_5", movie("INTRO", "intro2_f", "intro_6"));
  sceneManager.registerScene("intro_6", movie("HANGER1", "hang1_f", "intro_7"));
  sceneManager.registerScene("intro_7", movie("BRIDGE2", "brdg2_f", "intro_8"));
  sceneManager.registerScene("intro_8", movie("HANGER2", "neb2x_f", "intro_9"));
  sceneManager.registerScene("intro_9", movie("HANGER3", "intro5_f", "intro_10"));
  sceneManager.registerScene("intro_10", movie("BATTLE1B", "bat1b_f", "intro_11"));
  sceneManager.registerScene("intro_11", movie("BATTLE1", "bat1_f", "intro_12"));
  sceneManager.registerScene("intro_12", movie("BATTLE2", "bat2_f", "intro_13"));
  sceneManager.registerScene("intro_13", movie("BATTLE3", "bat3_f", "intro_14"));
  sceneManager.registerScene("intro_14", movie("BATTLE4", "bat4_f", "intro_15"));
  sceneManager.registerScene("intro_15", movie("XLOGO", "xlogo_f", "register"));
  // sceneManager.registerScene("intro_16", movie("CREDITS", "crdts_s", "register"));

  sceneManager.registerScene("register",
                             interactive<xwing::RegisterSceneController>("REGISTER", "pilot"));
  sceneManager.registerScene("mainmenu",
                             interactive<xwing::MainMenuSceneController>("MAINMENU", "mainmenu"));

  sceneManager.switchToScene("intro_1");

  return engine.run() ? 0 : 1;
}
