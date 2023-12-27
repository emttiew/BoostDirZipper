#include "DataEntry.hpp"

namespace archive_utils
{
    void DataEntry::writeToStream(io::filtering_ostream &out)
    {
        out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        out.write(dataBuffer.data(), dataSize);
    }

    void DataEntry::readFromStream(io::filtering_istream &in)
    {
        in.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        dataBuffer.resize(dataSize);
        in.read(dataBuffer.data(), dataSize);
    }
}