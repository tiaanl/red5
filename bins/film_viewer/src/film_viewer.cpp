#include <SDL.h>
#include <engine/engine.h>
#include <game/resources.h>
#include <game/scene_stage.h>

class SceneViewerController : public game::SceneController {
public:
  explicit SceneViewerController(std::string_view name, std::vector<std::string> resourceFiles)
    : m_name{name}, m_resourceFiles{std::move(resourceFiles)} {}

  bool setUpScene(game::Scene& scene) override {
    for (auto& resourceFile : m_resourceFiles) {
      resources.addResourceFile({resourceFile});
    }

    if (!scene.loadPalette("standard")) {
      return false;
    }

    return scene.loadFilm(m_name);
  }

private:
  std::string m_name;
  std::vector<std::string> m_resourceFiles;
};

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Film Viewer")) {
    return 1;
  }

  /*
  auto gameStageState = game::GameStageState::create(&engine.renderer());
  gameStageState->resources.addResourceFile({R"(C:\xwing\RESOURCE\MAINMENU.LFD)"});
  engine.setStage(
      game::createGameStage<game::SingleSceneStage>(std::move(gameStageState), "mainmenu"));
  */

  auto gameStageState = game::GameStageState::create(&engine.renderer());

  std::vector<std::string> resourceFiles = {R"(C:\xwing\RESOURCE\XWING.LFD)",
                                            R"(C:\xwing\RESOURCE\MAINMENU.LFD)"};
  auto controller = std::make_unique<SceneViewerController>("mainmenu", std::move(resourceFiles));
  auto stage = std::make_unique<game::SceneStage>(std::move(gameStageState), std::move(controller));
  engine.setStage(std::move(stage));

  return engine.run() ? 0 : 1;
}

#if 0
// std::vector<std::string> films = {"logo_f", "along", "bat1_f", "bat2_f", "bat3_f", "bat4_f",
  // "brdg1_f", "brdg2_f", "intro5_f", "xlogo_f"};
  std::vector<std::string> films = {
      "ass1_f",    //
      "ass2_f",    //
      "ass2_l",    //
      "ass3_f",    //
      "award1_f",  //
      "award1_s",  //
      "award2_f",  //
      "award2_s",  //
      "award3_f",  //
      "award3_f",  //
      "award3_s",  //
      "award3_s",  //
      "bat1_f",    //
      "bat1b_f",   //
      "bat2_f",    //
      "bat3_f",    //
      "bat4_f",    //
      "bint1_f",   //
      "bint1_s",   //
      "bint2_f",   //
      "blueprnt",  //
      "boom1_f",   //
      "boom2_f",   //
      "boom2_s",   //
      "boom3_f",   //
      "boom3_l",   //
      "brdg1_f",   //
      "brdg1_s",   //
      "brdg2_f",   //
      "brdg2_s",   //
      "brief_f",   //
      "caret_f",   //
      "cawing_f",  //
      "cbret_f",   //
      "cbrief_f",  //
      "cbwing_f",  //
      "combat",    //
      "crdts_s",   //
      "cs-4a-1",   //
      "csret_f",   //
      "cxret_f",   //
      "cxwing_f",  //
      "cyret_f",   //
      "cywing_f",  //
      "darth04",   //
      "decombat",  //
      "detour",    //
      "detrain",   //
      "dfire1_f",  //
      "dfire1_s",  //
      "dfire2_f",  //
      "dfire2_s",  //
      "dfire3_f",  //
      "dfire3_s",  //
      "dfire4_f",  //
      "dfire4_s",  //
      "dfire5_f",  //
      "dfire5_s",  //
      "docka_f",   //
      "docka_s",   //
      "dockb_f",   //
      "dockb_s",   //
      "dockc_f",   //
      "dockc_s",   //
      "dsbay_f",   //
      "dsbay_s",   //
      "dsboom_f",  //
      "dsdn4a_f",  //
      "dsdn4a_s",  //
      "dsdn4b_f",  //
      "dsdn4c_f",  //
      "dsdn4c_s",  //
      "dsdone_f",  //
      "dsdone_s",  //
      "dsf-s3f2",  //
      "dsland_f",  //
      "dsland_s",  //
      "empf_f",    //
      "empf_s",    //
      "exit_f",    //
      "exit_s",    //
      "filmload",  //
      "filmview",  //
      "fun1_f",    //
      "fun2_f",    //
      "fun2_s",    //
      "gov1_f",    //
      "gov1_s",    //
      "gov2_f",    //
      "gov3_f",    //
      "gov3_s",    //
      "gov4_f",    //
      "gov5_f",    //
      "gov6_f",    //
      "hang1_f",   //
      "hang1_l",   //
      "hang2a_f",  //
      "hang2a_s",  //
      "hang2b_f",  //
      "hang2b_s",  //
      "hang2x_f",  //
      "hang2x_s",  //
      "hang2y_f",  //
      "hang2y_s",  //
      "hang3a_f",  //
      "hang3a_s",  //
      "hang3b_f",  //
      "hang3b_s",  //
      "hang3x_f",  //
      "hang3x_s",  //
      "hang3y_f",  //
      "hang3y_s",  //
      "hoth1_f",   //
      "hoth2_f",   //
      "hoth2_s",   //
      "hoth3_f",   //
      "hyper_f",   //
      "ifoption",  //
      "inflight",  //
      // "intro1_f",  //
      // "intro1_s",  //
      // "intro2_f",  //
      // "intro2_s",  //
      "intro5_f",  //
      "intro5_s",  //
      "landa_f",   //
      "landa_s",   //
      "landb_f",   //
      "landb_s",   //
      "landx_f",   //
      "landx_s",   //
      "landy_f",   //
      "landy_s",   //
      "leave1_f",  //
      "leaveb_f",  //
      "leaveb_s",  //
      "log",       //
      "logo_f",    //
      "logo_f",    //
      "logo_s",    //
      "logo_s",    //
      "mainmenu",  //
      "medal1_f",  //
      "medal2_f",  //
      "medal2_s",  //
      "medal3_f",  //
      "medal3_s",  //
      "medals",    //
      "medic_f",   //
      "medic_s",   //
      "neb2a_f",   //
      "neb2a_s",   //
      "neb2b_f",   //
      "neb2b_s",   //
      "neb2x_f",   //
      "neb2x_s",   //
      "neb2y_f",   //
      "neb2y_s",   //
      "nebla_f",   //
      "nebla_s",   //
      "neblb_f",   //
      "neblb_s",   //
      "neblx_f",   //
      "neblx_s",   //
      "nebly_f",   //
      "nebly_s",   //
      "nrepa_f",   //
      "nrepa_s",   //
      "nrepb_f",   //
      "nrepb_s",   //
      "nrepx_f",   //
      "nrepx_s",   //
      "nrepy_f",   //
      "nrepy_s",   //
      "out1_f",    //
      "out2a_f",   //
      "out2a_s",   //
      "out2b_f",   //
      "out2b_s",   //
      "out3_f",    //
      "out3_s",    //
      "out4_f",    //
      "out4_s",    //
      "out5_f",    //
      "pilot",     //
      "pilot",     //
      "plans1_f",  //
      "plans1_s",  //
      "plans2_f",  //
      "plans3_f",  //
      "plans3_s",  //
      "plans4_f",  //
      "plans4_s",  //
      "prison_f",  //
      "prison_s",  //
      "probe1_f",  //
      "probe2_f",  //
      "probe3_f",  //
      "reb_f",     //
      "reb_s",     //
      "repa_f",    //
      "repa_s",    //
      "repb_f",    //
      "repb_s",    //
      "repx_f",    //
      "repx_s",    //
      "repy_f",    //
      "repy_s",    //
      "resci_f",   //
      "resci_s",   //
      "rescr_f",   //
      "rescr_s",   //
      "reta_f",    //
      "reta_s",    //
      "retb_f",    //
      "retb_s",    //
      "retx_f",    //
      "retx_s",    //
      "rety_f",    //
      "rety_s",    //
      "sab1_f",    //
      "sab1_s",    //
      "sab2_f",    //
      "sab2_s",    //
      "sab3_f",    //
      "sab3_l",    //
      "sab3_s",    //
      "taret_f",   //
      "tawing_f",  //
      "tbret_f",   //
      "tbwing_f",  //
      "tcfilm_f",  //
      "tcfilm_s",  //
      "tenda_f",   //
      "tendb_f",   //
      "tendx_f",   //
      "tendy_f",   //
      "tod",       //
      "totour_f",  //
      "totour_s",  //
      "train",     //
      "trfilm_f",  //
      "trfilm_s",  //
      "tsret_f",   //
      "ttfilm_f",  //
      "ttfilm_s",  //
      "txret_f",   //
      "txwing_f",  //
      "tyret_f",   //
      "tywing_f",  //
      "uniform",   //
      "xlogo_f",   //
      "xlogo_l",   //
      "yavin1_f",  //
      "yavin1_s",  //
      "yavin2_f",  //
      "yavin2_s",  //
      "yavin3_f",  //
      "yavin3_s",  //
  };
  engine.setStage(std::make_unique<CutsceneStage>(std::move(films)));
#endif  // 0
