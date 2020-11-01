#pragma once

#include <lfd/resource_file.h>

#include <vector>

namespace game {

class Resources {
public:
  Resources();

  std::vector<ResourceEntry>::const_iterator begin() const {
    return m_entries.begin();
  }

  std::vector<ResourceEntry>::const_iterator end() const {
    return m_entries.end();
  }

  bool addResourceFile(const ResourceFile& resourceFile);
  ResourceEntry* findResource(ResourceType type, std::string_view name);

private:
  std::vector<ResourceEntry> m_entries;
};

}  // namespace game
