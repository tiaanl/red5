#if 0
#include <nucleus/Streams/ArrayInputStream.h>
#include <nucleus/Streams/DynamicBufferOutputStream.h>
#include <resource/film.h>

#include "lfd/resource_file.h"

int main(int argc, char* argv[]) {
  const nu::FilePath& inPath = nu::FilePath{R"(C:\xwing\RESOURCE\LOGO2.LFD)"};
  LOG(Info) << "inPath: " << inPath;
  ResourceFile logoFile{inPath};
  auto entries = logoFile.loadEntries();

  for (auto& entry : entries) {
    LOG(Info) << entry.type() << " - " << entry.name();

    if (entry.type() == ResourceType::Film && entry.name() == "logo_s") {
      nu::ArrayInputStream s{entry.data().view()};
      auto film = film::read(&s);

      nu::DynamicArray<film::Chunk> chunks;
      chunks.emplaceBack(film::OpCode::Time).element().vars = {0};
      chunks.emplaceBack(film::OpCode::Event).element().vars = {20};
      chunks.emplaceBack(film::OpCode::Display).element().vars = {1};
      chunks.emplaceBack(film::OpCode::Layer).element().vars = {50};
      chunks.emplaceBack(film::OpCode::Time).element().vars = {52};
      chunks.emplaceBack(film::OpCode::Display).element().vars = {0};
      chunks.emplaceBack(film::OpCode::End);
      film->blocks[5].chunks = std::move(chunks);

      nu::DynamicBufferOutputStream o;
      film::write(&o, *film);
      entry.data(o.buffer());
    }
  }

  const nu::FilePath& outPath = nu::FilePath{R"(C:\xwing\RESOURCE\LOGO.LFD)"};
  LOG(Info) << "outPath: " << outPath;
  ResourceFile logo2File{outPath};
  logo2File.saveEntries(entries);

  return 0;
}
#endif  // 0

#if 1
#include <base/streams/memory_input_stream.h>

#include <iostream>

#include "base/logging.h"
#include "lfd/animation.h"
#include "lfd/film.h"
#include "lfd/image.h"
#include "lfd/palette.h"
#include "lfd/resource_file.h"

int main(int argc, char* argv[]) {
  auto resourcePath = std::filesystem::path{R"(C:\xwing\RESOURCE)"};

  lg::info("Resource Path: {}", resourcePath.string());

  std::vector<std::filesystem::path> resourceFiles;
  for (auto& dirEntry : std::filesystem::recursive_directory_iterator(resourcePath)) {
    resourceFiles.emplace_back(dirEntry.path());
  }

  for (auto& path : resourceFiles) {
    if (path.extension() == "LFD") {
      continue;
    }

    ResourceFile resourceFile = ResourceFile{path};
    auto entries = resourceFile.loadEntries();

    for (auto& entry : entries) {
      lg::info("entry: {} {}", resourceTypeToString(entry.type()), entry.name());

      base::MemoryInputStream stream{entry.data().data(), entry.data().size()};
      // nu::DynamicBufferOutputStream outStream;

      //      if (entry.type() == ResourceType::Film) {
      //        Film f;
      //        f.read(&stream, entry.data().size());
      //        f.write(&outStream);
      //      } else if (entry.type() == ResourceType::Palette) {
      //        Palette p;
      //        p.read(&stream);
      //        p.write(&outStream);
      //      } else
      if (entry.type() == ResourceType::Image) {
        //        LOG(Info) << entry;

        Image p;
        p.read(&stream, entry.data().size());
        // p.write(&outStream);
      } else if (entry.type() == ResourceType::Animation) {
        //        LOG(Info) << entry;

        Animation a;
        a.read(&stream, entry.data().size());
      } else {
        continue;
      }

#if 0
      if (entry.data().size() != outStream.buffer().size()) {
        LOG(Warning) << "Buffers are different sizes (" << entry.data().size() << " vs "
                     << outStream.buffer().size() << ")";
      }

      U8* left = (U8*)entry.data().data();
      U8* right = (U8*)outStream.buffer().data();
      for (auto i = 0u; i < entry.data().size(); ++i) {
        if (left[i] == right[i]) {
          continue;
        }

        LOG(Error) << "bytes not the same at pos: " << i << " (" << (I32)left[i] << " vs "
                   << (I32)right[i] << ")";
        break;
      }
#endif  // 0
    }
  }

  return 0;
}
#endif
