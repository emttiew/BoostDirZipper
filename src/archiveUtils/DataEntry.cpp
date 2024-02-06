#include "DataEntry.hpp"

namespace archive_utils
{
    void DataEntry::writeToStream(io::filtering_ostream &out)
    {
        std::cout << "writing " << dataSize << "into stream" << std::endl;
        out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        out.write(dataBuffer.data(), dataSize);
    }

    void DataEntry::readFromStream(io::filtering_istream &in)
    {
        in.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        std::cout << "reading " << dataSize << "into buffer from stream" << std::endl;
        dataBuffer.resize(dataSize);
        in.read(dataBuffer.data(), dataSize);
    }
}