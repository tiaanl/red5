#pragma once

#include <filesystem>
#include <vector>

#include "lfd/resource_type.h"

std::ostream& operator<<(std::ostream& os, ResourceType resourceType);

class ResourceEntry {
public:
  ResourceEntry(ResourceType type, std::string_view name, std::vector<U8> data)
    : m_type{type}, m_name{name}, m_data{std::move(data)} {}

  ResourceType type() const {
    return m_type;
  }

  std::string_view name() const {
    return m_name;
  }

  const std::vector<U8>& data() const {
    return m_data;
  }

  void data(std::vector<U8> data) {
    m_data = std::move(data);
  }

private:
  ResourceType m_type;
  std::string m_name;
  std::vector<U8> m_data;
};

class ResourceFile {
public:
  ResourceFile(std::filesystem::path path) : m_path{std::move(path)} {}

  std::vector<ResourceEntry> loadEntries() const;
  void saveEntries(const std::vector<ResourceEntry>& entries);

private:
  std::filesystem::path m_path;
};

inline const ResourceEntry* findResource(const std::vector<ResourceEntry>& entries,
                                         ResourceType resourceType, std::string_view name) {
  auto it = std::find_if(std::begin(entries), std::end(entries), [&](const ResourceEntry& entry) {
    return entry.type() == resourceType &&
           entry.name() == std::string_view{name.data(), name.size()};
  });

  if (it == std::end(entries)) {
    return nullptr;
  }

  return &*it;
}

template <typename T>
inline std::unique_ptr<T> loadResource(const std::vector<ResourceEntry>& entries,
                                       ResourceType resourceType, std::string_view name) {
  auto* resource = findResource(entries, resourceType, name);
  if (!resource) {
    return {};
  }

  // LOG(Info) << "Resource size: " << resource->data().size();

//  nu::ArrayInputStream stream{nu::ArrayView{resource->data().data(), resource->data().size()}};
//
//  auto result = std::make_unique<T>();
//  result->read(&stream, resource->data().size());
//
//  return result;

  return {};
}
