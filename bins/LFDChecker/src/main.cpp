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
#include <lfd/animation.h>
#include <nucleus/Streams/ArrayInputStream.h>
#include <nucleus/Streams/DynamicBufferOutputStream.h>

#include "lfd/film.h"
#include "lfd/image.h"
#include "lfd/palette.h"
#include "lfd/resource_file.h"

int main(int argc, char* argv[]) {
  auto resourcePath = nu::FilePath(R"(C:\xwing\RESOURCE)");

  LOG(Info) << "Resource path: " << resourcePath;

  auto resourceFiles = nu::findAllFilesIn(resourcePath);
  if (resourceFiles.isEmpty()) {
    LOG(Error) << "No resource files found!";
    return 1;
  }

  // LOG(Info) << resourceFiles;

  //  auto resourceFilePath = nu::filter(
  //      resourceFiles, [](const nu::FilePath& path) { return path.getPath().contains("LOGO");
  //      })[0];
  for (auto& resourceFilePath : resourceFiles) {
    if (!resourceFilePath.getPath().contains("LFD")
        // || !resourceFilePath.getPath().contains("BOX")
    ) {
      continue;
    }

    // LOG(Info) << resourceFilePath;

    ResourceFile resourceFile = ResourceFile{resourceFilePath};
    auto entries = resourceFile.loadEntries();

    for (auto& entry : entries) {
      // LOG(Info) << entry;

      nu::ArrayInputStream stream{nu::ArrayView<U8>{entry.data()}};
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
