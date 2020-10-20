#pragma once

#include <base/streams/memory_input_stream.h>

#include <filesystem>
#include <optional>
#include <vector>

#include "lfd/resource_type.h"

namespace base {
class InputStream;
}  // namespace base

class ResourceEntry {
public:
  ResourceEntry(ResourceType type, std::string name, std::vector<U8> data)
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
  static std::optional<ResourceEntry> readResourceEntry(base::InputStream* stream);

  std::filesystem::path m_path;
};

template <typename T>
inline std::unique_ptr<T> loadResource(const ResourceEntry& entry) {
  base::MemoryInputStream stream{entry.data()};
  auto result = std::make_unique<T>();
  result->read(&stream, entry.data().size());

  return result;
}
