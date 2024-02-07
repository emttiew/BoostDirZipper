#pragma once

#include "Entry.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

namespace archive_utils
{
    class FileEntry : Entry
    {
    public:
        explicit FileEntry(std::size_t pDataSize) : dataSize(pDataSize)
        {
            dataBuffer.reserve(dataSize);
        }

        explicit FileEntry(io::filtering_istream &in)
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

        void writeToStream(io::filtering_ostream &out) override;
        void readFromStream(io::filtering_istream &in) override;

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
