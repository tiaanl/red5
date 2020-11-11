#include <base/streams/memory_input_stream.h>
#include <lfd/animation.h>
#include <lfd/craft.h>
#include <lfd/film.h>
#include <lfd/font.h>
#include <lfd/palette.h>
#include <lfd/resource_file.h>

int main(int argc, char* argv[]) {
  auto resourcePath = std::filesystem::path{R"(C:\xwing\RESOURCE)"};

  spdlog::info("Resource Path: {}", resourcePath.string());

  std::vector<std::filesystem::path> resourceFiles;
  for (auto& dirEntry : std::filesystem::recursive_directory_iterator(resourcePath)) {
    resourceFiles.emplace_back(dirEntry.path());
  }

  for (auto& path : resourceFiles) {
    if (path.extension() != ".LFD") {
      continue;
    }

    if (path.filename() != "SPECIES.LFD") {
      continue;
    }

    ResourceFile resourceFile = ResourceFile{path};
    auto entries = resourceFile.loadEntries();

    for (auto& entry : entries) {
      spdlog::info("Entry :: ({}) {} ({} bytes)", resourceTypeToString(entry.type()), entry.name(),
                   entry.data().size());

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
          lfd::Film film;
          film.read(&stream, entry.data().size());
          break;
        }

        case ResourceType::Font: {
          lfd::Font font;
          font.read(&stream, entry.data().size());
        }

        case ResourceType::Craft: {
          lfd::Craft craft;
          craft.read(&stream, entry.data().size());
          return 0;
        }

        default: {
          break;
        }
      }

      auto bytesRead = stream.getPosition() - startPosition;

      // Check that the whole stream was consumed.
      if (bytesRead < entry.data().size()) {
        spdlog::warn("Resource contained {} bytes, but only {} bytes were read.",
                     entry.data().size(), stream.getPosition() - startPosition);
      }
    }
  }

  return 0;
}
