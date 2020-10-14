#include <Resources/ResourceFile.h>
#include <nucleus/Filter.h>
#include <nucleus/Streams/ArrayInputStream.h>
#include <resource/film.h>

int main(int argc, char* argv[]) {
  auto resourcePath = nu::FilePath(
      R"(C:\Program Files (x86)\Steam\steamapps\common\STAR WARS X-Wing\classic\RESOURCE)");

  LOG(Info) << "Resource path: " << resourcePath;

  auto resourceFiles = nu::findAllFilesIn(resourcePath);
  if (resourceFiles.isEmpty()) {
    LOG(Error) << "No resource files found!";
    return 1;
  }

  // LOG(Info) << resourceFiles;

  auto resourceFilePath = nu::filter(
      resourceFiles, [](const nu::FilePath& path) { return path.getPath().contains("AWARDS"); })[0];

  ResourceFile resourceFile = ResourceFile{resourceFilePath};
  auto entries = resourceFile.loadEntries();

  for (auto& entry : entries) {
    if (entry.type() != ResourceType::Film) {
      continue;
    }

    LOG(Info) << entry;

    nu::ArrayInputStream stream{nu::ArrayView<U8>{entry.data()}};
    film::load(&stream);
  }

  return 0;
}
