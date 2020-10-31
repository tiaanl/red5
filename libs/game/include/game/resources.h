#pragma once

#include <vector>

#include <lfd/resource_file.h>

namespace game {

class Resources {
public:
  Resources();

  bool addResourceFile(const ResourceFile& resourceFile);
  ResourceEntry* findResource(ResourceType type, std::string_view name);

private:
  std::vector<ResourceEntry> m_entries;
};

}  // namespace game
