#include "DataEntry.hpp"

namespace archive_utils
{
    DataEntry::DataEntry(std::size_t pDataSize, std::ifstream &file) : dataSize(pDataSize)
    {
        dataBuffer.resize(dataSize);
        if (file)
            file.read(dataBuffer.data(), dataSize);
    }

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

    std::vector<char> DataEntry::getData() const
    {
        return dataBuffer;
    }

    std::size_t DataEntry::getDataSize() const
    {
        return dataSize;
    }
}