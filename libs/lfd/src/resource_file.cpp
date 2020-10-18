#include "../include/lfd/resource_file.h"

#include <base/logging.h>

#include "base/streams/file_input_stream.h"
#include "base/streams/file_output_stream.h"

namespace {

struct Header {
  ResourceType type;
  U8 name[8];
  U32 size;
};

}  // namespace

std::ostream& operator<<(std::ostream& os, ResourceType resourceType) {
  os << resourceTypeToString(resourceType);
  return os;
}

std::vector<ResourceEntry> ResourceFile::loadEntries() const {
  lg::info("Loading entries from resource file: {}", m_path.string());

  base::FileInputStream stream{m_path};

  Header mainHeader{};
  stream.read(&mainHeader, sizeof(mainHeader));
  MemSize entryCount = mainHeader.size / sizeof(Header);

  lg::info("header: {}", mainHeader.size);

  // Skip the header block.
  stream.setPosition(stream.getPosition() + mainHeader.size);

  std::vector<ResourceEntry> entries;
  entries.reserve(entryCount);

  for (MemSize i = 0; i < entryCount; ++i) {
    Header header{};
    stream.read(&header, sizeof(header));

    std::vector<U8> data;
    data.resize(header.size);
    stream.read(data.data(), header.size);

    MemSize nameLength = 8;
    for (MemSize j = 0; j < 8; ++j) {
      if (!header.name[j]) {
        nameLength = j;
        break;
      }
    }
    std::string_view nameStr{reinterpret_cast<const char*>(header.name), nameLength};
    entries.emplace_back(header.type, nameStr, std::move(data));
  }

  return entries;
}

void writeHeader(base::OutputStream* stream, ResourceType resourceType, std::string_view name,
                 MemSize size) {
  stream->writeU32(static_cast<U32>(resourceType));
  U8 nameBuf[8] = {};
  std::memcpy(nameBuf, name.data(), std::min(name.length(), sizeof(nameBuf)));
  stream->write(nameBuf, sizeof(nameBuf));
  stream->writeU32(static_cast<U32>(size));
}

void ResourceFile::saveEntries(const std::vector<ResourceEntry>& entries) {
  base::FileOutputStream stream{m_path};

  writeHeader(&stream, ResourceType::ResourceMap, "resource", entries.size() * 16);

  for (auto& entry : entries) {
    writeHeader(&stream, entry.type(), entry.name(), entry.data().size());
  }

  for (auto& entry : entries) {
    writeHeader(&stream, entry.type(), entry.name(), entry.data().size());
    stream.write(entry.data().data(), entry.data().size());
  }
}
