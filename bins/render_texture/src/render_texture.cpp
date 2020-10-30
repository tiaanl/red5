
#include <engine/engine.h>

class MyStage : public engine::Stage {
public:
  bool onReady() override {
    U8 data[] = {
        255, 0,   0,   255,  // red
        0,   255, 0,   255,  // green
        0,   0,   255, 255,  // blue
        255, 255, 255, 255,  // white
    };
    m_texture = renderer()->renderer()->textures().create(data, {2, 2});
    if (!m_texture.isValid()) {
      return false;
    }

    return true;
  }

  void onUpdate(U32 millis) override {}

  void onRender() override {
    renderer()->render({0, 0}, {m_texture, {2, 2}});
  }

private:
  renderer::TextureId m_texture;
};

int main(int argc, char** argv) {
  engine::Engine engine;
  if (!engine.init("Render Texture")) {
    return 1;
  }

  engine.setStage(std::make_unique<MyStage>());

  engine.run();

  return 0;
}
