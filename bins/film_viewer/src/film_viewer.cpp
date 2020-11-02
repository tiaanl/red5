#include <SDL.h>
#include <engine/engine.h>
#include <game/resources.h>
#include <game/single_scene_stage.h>

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Film Viewer")) {
    return 1;
  }

  auto stage =
      std::make_unique<game::SingleSceneStage>(std::make_shared<game::Resources>(), "xlogo_f");
  stage->addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\XWING.LFD)"});
  stage->addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\XLOGO.LFD)"});

  if (!engine.setStage(std::move(stage))) {
    return 1;
  }

  engine.run();

  return 0;
}
