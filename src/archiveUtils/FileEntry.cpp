#include "FileEntry.hpp"

namespace archive_utils {
void FileEntry::writeToStream(io::filtering_ostream &out) {
  directory.writeToStream(out);
  out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
  out.write(dataBuffer.data(), dataSize);
}

void FileEntry::readFromStream(io::filtering_istream &in) {
  in.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
  dataBuffer.resize(dataSize);
  in.read(dataBuffer.data(), dataSize);
}

void FileEntry::writeMetaData(io::filtering_ostream &out) {
  directory.writeToStream(out);
  out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
}

void FileEntry::writeSome(io::filtering_ostream &out,
                          DataBufferType const &buffer,
                          std::streamsize const streamSize) {
  out.write(buffer.data(), streamSize);
}
}  // namespace archive_utils