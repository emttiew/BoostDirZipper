#pragma once

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "DirectoryEntry.hpp"
#include "FileEntry.hpp"

namespace fs = boost::filesystem;

namespace archive_utils {
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
