#if 1
#include <Resources/ResourceFile.h>
#include <nucleus/Streams/ArrayInputStream.h>
#include <nucleus/Streams/DynamicBufferOutputStream.h>
#include <resource/film.h>

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

//      film->blocks[5].chunks[0].layer.p1 = 0;
//      film->blocks[5].chunks[0].layer.p2 = 0;

//      nu::DynamicBufferOutputStream o;
//      film::write(&o, *film);
//      entry.data(o.buffer());
    }
  }

//  const nu::FilePath& outPath = nu::FilePath{R"(C:\xwing\RESOURCE\LOGO.LFD)"};
//  LOG(Info) << "outPath: " << outPath;
//  ResourceFile logo2File{outPath};
//  logo2File.saveEntries(entries);

  return 0;
}
#endif  // 0

#if 0
#include <Resources/ResourceFile.h>
#include <nucleus/Streams/ArrayInputStream.h>
#include <nucleus/Streams/DynamicBufferOutputStream.h>
#include <resource/film.h>

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
    if (!resourceFilePath.getPath().contains("LFD") ||
        !resourceFilePath.getPath().contains("BOX")) {
      continue;
    }

    LOG(Info) << resourceFilePath;

    ResourceFile resourceFile = ResourceFile{resourceFilePath};
    auto entries = resourceFile.loadEntries();

    for (auto& entry : entries) {
      if (entry.type() != ResourceType::Film) {
        continue;
      }

      // LOG(Info) << entry;

      nu::ArrayInputStream stream{nu::ArrayView<U8>{entry.data()}};
      auto f = film::read(&stream);

      nu::DynamicBufferOutputStream outStream;
      film::write(&outStream, *f);

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
    }
  }

  return 0;
}
#endif
