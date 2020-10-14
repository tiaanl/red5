#include "Resources/ResourceFile.h"

#include <nucleus/Streams/FileInputStream.h>
#include <nucleus/Text/StringView.h>

namespace {

const U8 g_resourceTypes[][4] = {
    {'D', 'E', 'L', 'T'},
    {'A', 'N', 'I', 'M'},
    {'F', 'I', 'L', 'M'},
};

struct Header {
  U8 type[4];
  U8 name[8];
  U32 size;
};

void readHeader(nu::InputStream* stream, Header* header) {
  stream->read(header, sizeof(Header));
}

std::ostream& operator<<(std::ostream& os, const Header& header) {
  char typeStr[sizeof(Header::type) + 1] = {};
  char nameStr[sizeof(Header::name) + 1] = {};

  std::memcpy(typeStr, header.type, sizeof(Header::type));
  std::memcpy(nameStr, header.name, sizeof(Header::name));

  os << '[' << typeStr << ", " << nameStr << ", " << header.size << ']';

  return os;
}

ResourceType toResourceType(U8* buf) {
  for (U32 i = 0; i < NU_ARRAY_SIZE(g_resourceTypes); ++i) {
    if (std::memcmp(buf, g_resourceTypes[i], 4) == 0) {
      return static_cast<ResourceType>(i);
    }
  }

  return ResourceType::Unknown;
}

}  // namespace

nu::DynamicArray<ResourceEntry> ResourceFile::loadEntries() const {
  LOG(Info) << "Loading entries from resource file: " << m_path;

  nu::FileInputStream stream{m_path};

  Header mainHeader{};
  readHeader(&stream, &mainHeader);
  MemSize entryCount = mainHeader.size / sizeof(Header);

  // Skip the header block.
  stream.setPosition(stream.getPosition() + mainHeader.size);

  nu::DynamicArray<ResourceEntry> entries;
  entries.reserve(entryCount);

  for (MemSize i = 0; i < entryCount; ++i) {
    Header header{};
    readHeader(&stream, &header);

    ResourceType type = toResourceType(header.type);

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
    entries.emplaceBack(type, nameStr, std::move(data));
  }

  return entries;
}

void ResourceFile::saveEntries(const nu::DynamicArray<ResourceEntry>& entries) {}
