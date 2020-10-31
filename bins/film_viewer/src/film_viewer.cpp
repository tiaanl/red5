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
      std::make_unique<game::SingleSceneStage>(std::make_shared<game::Resources>(), "uniform");
  stage->addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\XWING.LFD)"});
  stage->addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\UNIFORM.LFD)"});

  engine.setStage(std::move(stage));

  engine.run();

  return 0;
}
