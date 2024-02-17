#include "ArchiveUtils.hpp"

#include <cassert>
#include <iostream>

#include "ArchiveCompressor.hpp"
#include "ArchiveDecompressor.hpp"

namespace archive_utils {
namespace io = boost::iostreams;

void compressDirectory(const fs::path &inputDir, const fs::path &outputDir) {
  ArchiveCompressor archive(outputDir, inputDir);

  // TODO support single file compressing
  if (fs::is_regular_file(inputDir)) {
    throw std::runtime_error("error, provided path is not a folder " +
                             inputDir.string());
  } else {
    for (fs::recursive_directory_iterator it(inputDir), end; it != end; ++it) {
      if (fs::is_regular_file(*it)) {
        std::cout << "file path: " << it->path().string() << std::endl;
        archive.addFile(it->path());
      }
      if (fs::is_directory(*it)) {
        std::cout << "dirctory path: " << it->path().string() << std::endl;
        archive.addDirectory(it->path());
      }
    }
  }
}

namespace {
void ensureDirectoryExists(fs::path const &dir) {
  if (!fs::exists(dir)) fs::create_directories(dir);
}
}  // namespace

void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir) {
  ArchiveDecompressor archive(inputDir);
  ensureDirectoryExists(outputDir);

  archive.decompress();

  std::cout << "entries size " << archive.getEntries().size() << std::endl;

  for (EntryPtr const &entry : archive.getEntries()) {
    assert(entry);
    auto filepath = outputDir / fs::path{entry->getPath()}.relative_path();
    std::cout << "decompressing: " << filepath.string() << std::endl;
    if (entry->isDirectory()) {
      ensureDirectoryExists(filepath);
    } else {
      ensureDirectoryExists(filepath.parent_path());
      std::ofstream destFile(filepath.string().c_str(),
                    std::ios::binary | std::ios::trunc);
      if (destFile) {
        destFile.write(entry->getData(), entry->getDataSize());
        if (destFile.bad())
          throw std::runtime_error(
              "error while writing to file with decompressed data " +
              filepath.string());
        destFile.close();
      } else {
        throw std::runtime_error(
            "error while opening file for decompressed data " +
            filepath.string());
      }
    }
  }
}
}  // namespace archive_utils