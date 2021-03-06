#include "lfd/resource_file.h"

#include <base/streams/file_input_stream.h>
#include <base/streams/file_output_stream.h>

std::vector<ResourceEntry> ResourceFile::loadEntries() const {
  if (!std::filesystem::exists(m_path)) {
    spdlog::error("Resource file not found: {}", m_path.string());
    return {};
  }

  spdlog::info("Loading entries from resource file: {}", m_path.string());

  base::FileInputStream stream{m_path};
  // TODO: Check if the file was opened successfully.

  // Read ResourceMap header.
  auto type = static_cast<ResourceType>(stream.readU32());
  if (type != ResourceType::ResourceMap) {
    spdlog::error("Invalid LFD file. ({})", m_path.string());
    return {};
  }

  U8 name[9] = {};
  stream.read(name, 8);

  U32 size = stream.readU32();

#if 0
  spdlog::info("ResourceMap :: type: {}, name: {}, size: {}", resourceTypeToString(type), name, size);
#endif  // 0

  // Skip the header block and get the details from the individual resource headers.
  stream.advance(size);

  std::vector<ResourceEntry> entries;
  for (MemSize i = 0; i < size / 16; ++i) {
    auto resource = readResourceEntry(&stream);
    if (resource.has_value()) {
#if 0
      spdlog::info("entry: ({}) {}", resourceTypeToString(resource.value().type()),
                   resource.value().name());
#endif  // 0
      entries.emplace_back(std::move(resource.value()));
    }
  }

  return entries;
}

// static
std::optional<ResourceEntry> ResourceFile::readResourceEntry(base::InputStream* stream) {
  ResourceType type = static_cast<ResourceType>(stream->readU32());
  char name[9] = {};
  stream->read(name, 8);
  U32 size = stream->readU32();

  std::vector<U8> data;
  data.resize(size);

  stream->read(data.data(), size);

  return ResourceEntry{type, name, std::move(data)};
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
