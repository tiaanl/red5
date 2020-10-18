#include <base/logging.h>
#include <base/streams/memory_input_stream.h>
#include <lfd/animation.h>
#include <lfd/film.h>
#include <lfd/image.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>

#include <iostream>

int main(int argc, char* argv[]) {
  auto resourcePath = std::filesystem::path{R"(C:\xwing\RESOURCE)"};

  lg::info("Resource Path: {}", resourcePath.string());

  std::vector<std::filesystem::path> resourceFiles;
  for (auto& dirEntry : std::filesystem::recursive_directory_iterator(resourcePath)) {
    resourceFiles.emplace_back(dirEntry.path());
  }

  for (auto& path : resourceFiles) {
    if (path.extension() != ".LFD") {
      continue;
    }

    ResourceFile resourceFile = ResourceFile{path};
    auto entries = resourceFile.loadEntries();

    for (auto& entry : entries) {
      // lg::info("Entry :: ({}) {}", resourceTypeToString(entry.type()), entry.name());

      base::MemoryInputStream stream{entry.data()};
      auto startPosition = stream.getPosition();

      switch (entry.type()) {
        case ResourceType::Palette: {
          Palette palette;
          palette.read(&stream, entry.data().size());
          break;
        }

        case ResourceType::Image: {
          Image image;
          image.read(&stream, entry.data().size());
          break;
        }

        case ResourceType::Animation: {
          Animation animation;
          animation.read(&stream, entry.data().size());
          break;
        }

        case ResourceType::Film: {
          Film film;
          film.read(&stream, entry.data().size());
          break;
        }

        default: {
          continue;
        }
      }

      // Check that the whole stream was consumed.
      if (stream.getPosition() < entry.data().size()) {
        lg::warn("Resource contained {} bytes, but only {} bytes were read.", entry.data().size(),
                 stream.getPosition() - startPosition);
      }
    }
  }

  return 0;
}
