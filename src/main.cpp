#include <Resources/ResourceFile.h>
#include <nucleus/Filter.h>

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

  auto awards = nu::filter(resourceFiles, [](const nu::FilePath& path) {
    return path.getPath().contains("AWARDS");
  })[0];

  auto entries = ResourceFile{awards}.loadEntries();

  for (auto& entry : entries) {
    LOG(Info) << entry.name() << " -> " << entry.data().size();
  }

  return 0;
}
