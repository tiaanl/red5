#include "scene/scene_delegate.h"

namespace scene {

Delegate::~Delegate() = default;

void Delegate::onEvent(I16 event) {
  spdlog::info("Unhandled event: {}", event);
}

}  // namespace scene
