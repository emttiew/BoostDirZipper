#include "ArchiveCompressor.hpp"

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <stdexcept>

namespace {
const int BUFFER_SIZE = 1024;
}

namespace archive_utils {
namespace io = boost::iostreams;

ArchiveCompressor::ArchiveCompressor(const fs::path &outputDir,
                                     const fs::path &pInputDir)
    : inputDir(pInputDir) {
  archiveStream.push(io::gzip_compressor());
  archiveStream.push(
      io::file_descriptor_sink(outputDir.string(), std::ios::binary));
}

namespace {
void readWholeFile(const uintmax_t fileSize, std::string const &path,
                   std::ifstream &file, io::filtering_ostream &fileStream) {
  FileEntry::DataBufferType buffer(fileSize);
  file.read(buffer.data(), buffer.size());
  FileEntry fileEntry(buffer, path);
  fileEntry.writeToStream(fileStream);
}

void readInChunks(const uintmax_t fileSize, std::string const &path,
                  std::ifstream &file, io::filtering_ostream &fileStream) {
  FileEntry fileEntry(fileSize, path);
  FileEntry::DataBufferType buffer(BUFFER_SIZE);

  fileEntry.writeMetaData(fileStream);

  while (file.good()) {
    file.read(buffer.data(), buffer.size());
    std::streamsize dataSize = file.gcount();
    if (dataSize) fileEntry.writeSome(fileStream, buffer, dataSize);
  }
}
}  // namespace

void ArchiveCompressor::addFile(const fs::path &filePath) {
  // TODO check if the file is binary the same to previously added files
  std::ifstream file(filePath.string(), std::ios::binary);
  if (!file) throw std::runtime_error("cannot open input file");

  io::filtering_ostream fileStream;
  fileStream.push(archiveStream);

  std::string const &path = fs::relative(filePath, inputDir).string();
  uintmax_t fileSize = fs::file_size(filePath);
  if (fileSize < BUFFER_SIZE)
    readWholeFile(fileSize, path, file, fileStream);
  else
    readInChunks(fileSize, path, file, fileStream);

  if (fileStream.bad())
    throw std::runtime_error(
        "error while writing to compressing stream on file " + path);
  if (file.bad())
    throw std::runtime_error("error while reading file " + filePath.string());
}

void ArchiveCompressor::addDirectory(const fs::path &filePath) {
  io::filtering_ostream fileStream;
  fileStream.push(archiveStream);
  std::string const &path = fs::relative(filePath, inputDir).string();
  DirectoryEntry entry(path, EntryType::Directory);
  entry.writeToStream(fileStream);
  if (fileStream.bad())
    throw std::runtime_error(
        "error while writing to compressing stream on directory " + path);
}
}  // namespace archive_utils