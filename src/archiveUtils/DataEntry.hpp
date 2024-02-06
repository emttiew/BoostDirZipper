#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <cstring>
#include <fstream>
#include <memory>
#include <iostream>

namespace archive_utils
{
    namespace io = boost::iostreams;

    class DataEntry
    {
    public:
        explicit DataEntry(std::size_t pDataSize) : dataSize(pDataSize)
        {
            dataBuffer.reserve(dataSize);
        }

        explicit DataEntry(io::filtering_istream &in)
        {
            this->readFromStream(in);
        }

        void read(std::ifstream &file) // handle exception or file not opened
        {
            std::cout << "reading " << dataSize << "into buffer" << std::endl;
            file.read(dataBuffer.data(), dataSize);
        }

        const char *getData() const
        {
            return dataBuffer.data();
        }

        std::size_t getSize() const
        {
            return dataSize;
        }

        void writeToStream(io::filtering_ostream &out);
        void readFromStream(io::filtering_istream &in);

    private:
        std::size_t dataSize;
        std::vector<char> dataBuffer;
    };

    // struct DataEntry
    // {
    //     void writeToStream(io::filtering_ostream &out);
    //     void readFromStream(io::filtering_istream &in);
    //     std::size_t dataSize;
    //     std::vector<char> dataBuffer;
    // };
} // namespace archive_utils
