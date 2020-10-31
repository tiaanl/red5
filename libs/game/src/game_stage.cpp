#include "game/game_stage.h"

namespace game {

namespace {

constexpr I32 g_gameScreenWidth = 320;
constexpr I32 g_gameScreenHeight = 200;

}  // namespace

GameStage::GameStage(std::shared_ptr<Resources> resources) : m_resources{std::move(resources)} {}

bool GameStage::onLoad() {
  spdlog::info("GameStage::onLoad");
  m_gameScreen = m_renderer->renderTargets().create({g_gameScreenWidth, g_gameScreenHeight});
  if (!m_gameScreen) {
    spdlog::error("Could not create game screen render target.");
    return false;
  }

  return true;
}

void GameStage::onRender() {
  if (!m_gameScreen.isValid()) {
    return;
  }

  m_renderer->setRenderTarget(m_gameScreen);
  m_renderer->clear(0.0f, 0, 0, 1.0f);
  onRenderGameScreen();
  m_renderer->clearRenderTarget();

  m_renderer->clear(0.0f, 0.5f, 0.0f, 1.0f);
  auto screen = m_renderer->renderTarget();

  // spdlog::info("screen: ({}, {})", screen->size.width, screen->size.height);

  renderer::Rect destination{renderer::fitInto({0, 0, g_gameScreenWidth, g_gameScreenHeight},
                                               {0, 0, screen->size.width, screen->size.height})};

  m_renderer->renderRenderTarget(m_gameScreen, destination);
}

void GameStage::attachToEngine(renderer::Renderer* renderer) {
  Stage::attachToEngine(renderer);

  m_spriteRenderer.attachToRenderer(renderer);
}

void GameStage::detachFromEngine() {
  m_spriteRenderer.detachFromRenderer();

  Stage::detachFromEngine();
}

}  // namespace game
