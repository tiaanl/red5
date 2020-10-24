#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "USAGE: " << argv[0] << " <binary-file> <cpp-file>";
    return 0;
  }

  fs::path binaryPath{argv[1]};
  fs::path cppPath{argv[2]};

  spdlog::info("Converting {} to {}", binaryPath.string(), cppPath.string());

  std::vector<char> buffer;

  {
    std::ifstream stream{binaryPath, std::ios::binary};
    stream.seekg(0, std::ios::end);
    auto fileSize = stream.tellg();
    stream.seekg(0);
    buffer.resize(fileSize);
    stream.read(buffer.data(), buffer.size());
  }

  if (buffer.empty()) {
    spdlog::error("Could not read file or file is empty: {}", binaryPath.string());
    return 1;
  }

  std::string name{fs::path{cppPath}.replace_extension("").filename().string()};
  std::replace(std::begin(name), std::end(name), '-', '_');

  {
    std::ofstream stream{cppPath, std::ios::binary};
    stream << "extern const unsigned char " << name << "[];\n";
    stream << "const unsigned char " << name << "[] = {";
    for (char ch : buffer) {
      stream << "0x" << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<uint16_t>(static_cast<uint8_t>(ch)) << ",";
    }
    stream << "};";
  }

  return 0;
}
