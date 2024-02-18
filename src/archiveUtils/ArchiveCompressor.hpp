#pragma once

#include <filesystem>

#include "DirectoryEntry.hpp"
#include "FileEntry.hpp"

namespace archive_utils {
namespace fs = std::filesystem;
class ArchiveCompressor {
 public:
  ArchiveCompressor(const fs::path &outputDir, const fs::path &pInputDir);
  void addFile(const fs::path &filePath);
  void addDirectory(const fs::path &filePath);

 private:
  io::filtering_ostream archiveStream;
  fs::path inputDir;
};
}  // namespace archive_utils
