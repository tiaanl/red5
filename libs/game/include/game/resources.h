#pragma once

#include <lfd/resource_file.h>

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace game {

class Resources {
public:
  explicit Resources(fs::path resourceRoot);

  auto begin() const {
    return m_entries.begin();
  }

  auto end() const {
    return m_entries.end();
  }

  bool addResourceFile(std::string_view resourceFileName);
  ResourceEntry* findResource(ResourceType type, std::string_view name);

private:
  fs::path pathToResourceFile(std::string_view resourceFileName);

  fs::path m_resourceRoot;
  std::vector<ResourceEntry> m_entries;
};

}  // namespace game
