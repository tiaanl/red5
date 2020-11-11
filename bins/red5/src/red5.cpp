#include <engine/engine.h>
#include <xwing/stages/main_menu_stage.h>

#include <filesystem>

#include "xwing/stages/register_stage.h"

namespace fs = std::filesystem;

class CutsceneStage : public game::GameStage, public game::SceneDelegate {
public:
  CutsceneStage(std::shared_ptr<game::GameStageState> gameStageState,
                std::vector<std::string> films)
    : game::GameStage{std::move(gameStageState)}, m_films{std::move(films)} {}

  bool onLoad() override {
    loadFilm(m_films[m_currentFilmIndex]);

    return true;
  }

  void onUpdate(U32 millis) override {
    if (m_currentScene) {
      m_currentScene->update(millis);
    }
  }

  void onRenderGameScreen() override {
    if (m_currentScene) {
      m_currentScene->renderGameScreen();
    }
  }

  void onSceneLastFramePlayed() override {
    SceneDelegate::onSceneLastFramePlayed();

    spdlog::info("Last frame played");
    nextFilm();
  }

private:
  bool loadFilm(std::string_view name) {
    auto scene = std::make_unique<game::Scene>(this, &m_gameStageState->resources,
                                               &m_gameStageState->sceneRenderer);

    // Apply the default palette.
    if (!scene->loadPalette("standard")) {
      return false;
    }

    spdlog::info("Loading film: {}", name);

    // Load the specified film.
    if (!scene->loadFilm(name)) {
      return false;
    }

    m_currentScene = std::move(scene);

    return true;
  }

  bool nextFilm() {
    m_currentFilmIndex += 1;
    if (m_currentFilmIndex >= m_films.size()) {
      m_currentFilmIndex = 0;
    }

    return loadFilm(m_films[m_currentFilmIndex]);
  }

  std::vector<std::string> m_films;
  U32 m_currentFilmIndex = 0;
  std::unique_ptr<game::Scene> m_currentScene;
};

int main(int argc, char* argv[]) {
  engine::Engine engine;
  if (!engine.init("Red 5")) {
    return 1;
  }

#if defined(_WIN32)
  fs::path resourceRoot{R"(C:\xwing\RESOURCE)"};
#else
  fs::path resourceRoot{R"(/home/tilo/xwing/RESOURCE)"};
#endif

  auto gameStageState = game::GameStageState::create(&engine.renderer());
  gameStageState->resources.addResourceFile({resourceRoot / "XWING.LFD"});
  gameStageState->resources.addResourceFile({resourceRoot / "REGISTER.LFD"});
  gameStageState->resources.addResourceFile({resourceRoot / "MAINMENU.LFD"});
  engine.setStage(game::createGameStage<xwing::RegisterStage>(std::move(gameStageState)));

  return engine.run() ? 0 : 1;
}
