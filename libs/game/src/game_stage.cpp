#include "game/game_stage.h"

namespace game {

namespace {

constexpr I32 g_gameScreenWidth = 320;
constexpr I32 g_gameScreenHeight = 200;

}  // namespace

GameStage::GameStage(std::shared_ptr<Resources> resources) : m_resources{std::move(resources)} {}

void GameStage::onRenderDebugInfo() {}

bool GameStage::onLoad() {
  m_gameScreen = m_renderer->renderTargets().create({g_gameScreenWidth, g_gameScreenHeight});
  if (!m_gameScreen) {
    spdlog::error("Could not create game screen renderInternal target.");
    return false;
  }

  return true;
}

void GameStage::onStageResized(I32 width, I32 height) {
  Stage::onStageResized(width, height);

  auto screen = m_renderer->renderTarget();
  m_gameScreenRect = fitInto({0, 0, g_gameScreenWidth, g_gameScreenHeight},
                             {0, 0, screen->size.width, screen->size.height});
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
  m_renderer->renderRenderTarget(m_gameScreen, m_gameScreenRect);
  onRenderDebugInfo();
}

bool GameStage::attachToEngine(renderer::Renderer* renderer) {
  if (!Stage::attachToEngine(renderer)) {
    return false;
  }

  if (!m_sceneRenderer.attachToRenderer(renderer)) {
    return false;
  }

  return true;
}

void GameStage::detachFromEngine() {
  m_sceneRenderer.detachFromRenderer();

  Stage::detachFromEngine();
}

}  // namespace game
