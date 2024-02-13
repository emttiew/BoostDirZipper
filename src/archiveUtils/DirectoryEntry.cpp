#include "DirectoryEntry.hpp"

namespace archive_utils {
void DirectoryEntry::writeToStream(io::filtering_ostream &out) {
  out.put(static_cast<char>(entryType));
  out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
  out.write(filePath.data(), dataSize);
}

void DirectoryEntry::readFromStream(io::filtering_istream &in) {
  char temp;
  in.get(temp);
  entryType = static_cast<EntryType>(temp);
  in.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
  filePath.resize(dataSize);
  in.read(filePath.data(), dataSize);
}
}  // namespace archive_utils