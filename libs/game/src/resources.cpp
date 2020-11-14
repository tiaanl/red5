#include "game/resources.h"

namespace game {

Resources::Resources(fs::path resourceRoot) : m_resourceRoot{std::move(resourceRoot)} {}

bool Resources::addResourceFile(std::string_view resourceFileName) {
  auto path = pathToResourceFile(resourceFileName);

  auto entries = ResourceFile{path}.loadEntries();

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

fs::path Resources::pathToResourceFile(std::string_view resourceFileName) {
  std::string fileName{resourceFileName};
  fileName.append(".LFD");
  spdlog::info("fileName: {}", fileName);
  return m_resourceRoot / fileName;
}

}  // namespace game
