#include "ArchiveDecompressor.hpp"

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <stdexcept>

#include "DirectoryEntry.hpp"
#include "FileEntry.hpp"

namespace archive_utils {
ArchiveDecompressor::ArchiveDecompressor(const fs::path &inputDir) {
  archiveStream.push(io::gzip_decompressor());
  archiveStream.push(
      io::file_descriptor_source(inputDir.string(), std::ios::binary));
}

void ArchiveDecompressor::decompress() {
  while (archiveStream.peek() != EOF) {
    auto directoryEntry = std::make_shared<DirectoryEntry>();
    directoryEntry->readFromStream(archiveStream);

    if (archiveStream.bad())
      throw std::runtime_error("error while reading from decompressing stream");

    if (directoryEntry->isDirectory()) {
      entries.push_back(std::move(directoryEntry));
      continue;
    }
    auto fileEntry = std::make_shared<FileEntry>(*directoryEntry);
    fileEntry->readFromStream(archiveStream);
    if (archiveStream.bad())
      throw std::runtime_error("error while reading from decompressing stream");
    entries.push_back(std::move(fileEntry));
  }
}

EntryPtrVec ArchiveDecompressor::getEntries() const { return entries; }
}  // namespace archive_utils