#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

#include "DirectoryEntry.hpp"
#include "Entry.hpp"

namespace archive_utils {
class FileEntry : public Entry {
 public:
  using DataBufferType = std::vector<char>;
  FileEntry() : Entry(EntryType::File) {}
  FileEntry(DataBufferType const &buffer, std::string const &path)
      : Entry(EntryType::File, buffer.size()),
        dataBuffer(buffer),
        directory(path, EntryType::File) {}
  explicit FileEntry(DirectoryEntry const &pDirectory)
      : Entry(EntryType::File), directory(pDirectory) {}
  explicit FileEntry(size_t const dataSize, std::string const &path)
      : Entry(EntryType::File, dataSize), directory(path, EntryType::File) {}

  void writeMetaData(io::filtering_ostream &out);
  void writeSome(io::filtering_ostream &out, DataBufferType const &buffer,
                 std::streamsize const streamSize);
  void writeToStream(io::filtering_ostream &out) override;
  void readFromStream(io::filtering_istream &in) override;
  std::string getPath() const override { return directory.getPath(); }
  const char *getData() const override { return dataBuffer.data(); }

 private:
  DataBufferType dataBuffer;
  DirectoryEntry directory;
};
}  // namespace archive_utils
