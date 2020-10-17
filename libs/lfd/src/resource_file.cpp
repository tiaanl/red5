#include "../include/lfd/resource_file.h"

#include <nucleus/Streams/FileInputStream.h>
#include <nucleus/Streams/FileOutputStream.h>

namespace {

struct Header {
  ResourceType type;
  U8 name[8];
  U32 size;
};

void readHeader(nu::InputStream* stream, Header* header) {
  stream->read(header, sizeof(Header));
}

std::ostream& operator<<(std::ostream& os, const Header& header) {
  char nameStr[sizeof(Header::name) + 1] = {};

  std::memcpy(nameStr, header.name, sizeof(Header::name));

  os << '[' << resourceTypeToString(header.type) << ", " << nameStr << ", " << header.size << ']';

  return os;
}

}  // namespace

std::ostream& operator<<(std::ostream& os, ResourceType resourceType) {
  os << resourceTypeToString(resourceType);
  return os;
}

std::vector<ResourceEntry> ResourceFile::loadEntries() const {
  // LOG(Info) << "Loading entries from resource file: " << m_path;

  nu::FileInputStream stream{m_path};

  Header mainHeader{};
  readHeader(&stream, &mainHeader);
  MemSize entryCount = mainHeader.size / sizeof(Header);

  // Skip the header block.
  stream.setPosition(stream.getPosition() + mainHeader.size);

  std::vector<ResourceEntry> entries;
  entries.reserve(entryCount);

  for (MemSize i = 0; i < entryCount; ++i) {
    Header header{};
    readHeader(&stream, &header);

    nu::DynamicArray<U8> data;
    data.resize(header.size);
    stream.read(data.data(), header.size);

    MemSize nameLength = 8;
    for (MemSize j = 0; j < 8; ++j) {
      if (!header.name[j]) {
        nameLength = j;
        break;
      }
    }
    nu::StringView nameStr{reinterpret_cast<Char*>(header.name), nameLength};
    entries.emplace_back(header.type, nameStr, std::move(data));
  }

  return entries;
}

void writeHeader(nu::OutputStream* stream, ResourceType resourceType, nu::StringView name,
                 MemSize size) {
  stream->writeU32(static_cast<U32>(resourceType));
  U8 nameBuf[8] = {};
  std::memcpy(nameBuf, name.data(), std::min(name.length(), sizeof(nameBuf)));
  stream->write(nameBuf, sizeof(nameBuf));
  stream->writeU32(static_cast<U32>(size));
}

void ResourceFile::saveEntries(const nu::DynamicArray<ResourceEntry>& entries) {
  nu::FileOutputStream stream{m_path};

  writeHeader(&stream, ResourceType::ResourceMap, "resource", entries.size() * 16);

  for (auto& entry : entries) {
    writeHeader(&stream, entry.type(), entry.name(), entry.data().size());
  }

  for (auto& entry : entries) {
    writeHeader(&stream, entry.type(), entry.name(), entry.data().size());
    stream.write(entry.data().data(), entry.data().size());
  }
}
