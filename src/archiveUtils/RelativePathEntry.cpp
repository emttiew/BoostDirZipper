#include "RelativePathEntry.hpp"

namespace archive_utils
{
    RelativePathEntry::RelativePathEntry(char pIsRegularFile, const std::string &pFilePath)
        : isRegularFile(pIsRegularFile), pathSize(pFilePath.size()), filePath(pFilePath)
    {
    }

    bool RelativePathEntry::isDirectory() const
    {
        return isRegularFile == '\x00'; // TODO upgrade this value to a variable
    }

    void RelativePathEntry::writeToStream(io::filtering_ostream &out)
    {
        out.put(isRegularFile);
        out.write(reinterpret_cast<char *>(&pathSize), sizeof(pathSize));
        out.write(filePath.data(), pathSize);
    }

    void RelativePathEntry::readFromStream(io::filtering_istream &in)
    {
        in.get(isRegularFile);
        in.read(reinterpret_cast<char *>(&pathSize), sizeof(pathSize));
        filePath.resize(pathSize);
        in.read(filePath.data(), pathSize);
    }

    std::string RelativePathEntry::getFilePath() const
    {
        return filePath;
    }
}