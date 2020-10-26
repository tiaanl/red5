
#include <SDL.h>
#include <engine/engine.h>
#include <engine/single_scene_stage.h>

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Film Viewer")) {
    return 1;
  }

  engine.addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\XWING.LFD)"});
  engine.addResourceFile(ResourceFile{R"(C:\xwing\RESOURCE\UNIFORM.LFD)"});

  engine.setStage(std::make_unique<engine::SingleSceneStage>("uniform"));

  engine.run();

  return 0;
}
