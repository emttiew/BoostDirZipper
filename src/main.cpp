#include <filesystem>
#include <iostream>

#include "archiveUtils/ArchiveUtils.hpp"

int main(int argc, char *argv[]) {
  namespace fs = std::filesystem;
  if (argc < 3) {
    std::cout << "Usage: " << argv[0]
              << " c|compress|d|decompress <source_path> <sink_path>\n";
    return 1;
  }

  std::string mode = argv[1];
  std::string sourcePath = argv[2];
  std::string archivePath = argv[3];

  if (mode == "c" || mode == "compress") {
    fs::path source(sourcePath);
    fs::path archive(archivePath);
    archive_utils::compressDirectory(source, archive);
    std::cout << "Compression completed.\n";
  } else if (mode == "d" || mode == "decompress") {
    fs::path archive(sourcePath);
    fs::path destination(archivePath);
    archive_utils::decompressDirectory(archive, destination);
    std::cout << "Decompression completed.\n";
  } else {
    std::cout << "Invalid mode. Available modes: compress, decompress\n";
    return 1;
  }

  return 0;
}