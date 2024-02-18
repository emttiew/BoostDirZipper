#pragma once

#include <filesystem>
#include <memory>
#include <vector>

#include "Entry.hpp"

namespace archive_utils {
namespace fs = std::filesystem;
namespace io = boost::iostreams;
using EntryPtr = std::shared_ptr<Entry>;
using EntryPtrVec = std::vector<EntryPtr>;

class ArchiveDecompressor {
 public:
  explicit ArchiveDecompressor(const fs::path &inputDir);
  EntryPtrVec getEntries() const;
  void decompress();

 private:
  io::filtering_istream archiveStream;
  EntryPtrVec entries;
};
}  // namespace archive_utils