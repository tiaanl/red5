#if !defined(RESOURCES_RESOURCE_FILE_H_)
#define RESOURCES_RESOURCE_FILE_H_

#include <nucleus/Containers/DynamicArray.h>
#include <nucleus/FilePath.h>
#include <nucleus/Text/StaticString.h>

enum class ResourceType : U32 {
  Image,
  Animation,
  Film,

  Unknown,
};

class ResourceEntry {
public:
  ResourceEntry(ResourceType type, nu::StringView name, nu::DynamicArray<U8> data)
    : m_type{type}, m_name{name}, m_data{std::move(data)} {}

  ResourceType type() const {
    return m_type;
  }

  nu::StringView name() const {
    return m_name.view();
  }

  const nu::DynamicArray<U8>& data() {
    return m_data;
  }

  friend std::ostream& operator<<(std::ostream& os, const ResourceEntry& resourceEntry) {
    os << '[' << (int)resourceEntry.m_type << ", " << resourceEntry.m_name.view() << ']';
    return os;
  }

private:
  ResourceType m_type;
  nu::DynamicString m_name;
  nu::DynamicArray<U8> m_data;
};

class ResourceFile {
public:
  ResourceFile(const nu::FilePath& path) : m_path{path} {}

  nu::DynamicArray<ResourceEntry> loadEntries();
  void saveEntries(const nu::DynamicArray<ResourceEntry>& entries);

private:
  nu::FilePath m_path;
};

#endif  // RESOURCES_RESOURCE_FILE_H_
