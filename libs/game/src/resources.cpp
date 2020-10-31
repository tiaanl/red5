#include "game/resources.h"

namespace game {

Resources::Resources() = default;

bool Resources::addResourceFile(const ResourceFile& resourceFile) {
  auto entries = resourceFile.loadEntries();

  for (auto& entry : entries) {
    // spdlog::info("entry: {} ({})", entry.name(), resourceTypeToString(entry.type()));
    m_entries.emplace_back(std::move(entry));
  }

  return true;
}

ResourceEntry* Resources::findResource(ResourceType type, std::string_view name) {
  auto it = std::find_if(std::begin(m_entries), std::end(m_entries),
                         [&](const ResourceEntry& resourceEntry) {
                           return resourceEntry.type() == type && resourceEntry.name() == name;
                         });

  if (it == std::end(m_entries)) {
    spdlog::warn("Resource not found: {} ({})", name, resourceTypeToString(type));
    return nullptr;
  }

  return &*it;
}

}  // namespace game
