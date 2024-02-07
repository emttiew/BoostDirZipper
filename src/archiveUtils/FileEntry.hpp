#pragma once

#include "DirectoryEntry.hpp"
#include "Entry.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

namespace archive_utils
{
    class FileEntry : public Entry
    {
    public:
        using DataBufferType = std::vector<char>;
        FileEntry(DataBufferType const &buffer, std::string const &path) : Entry(buffer.size()), dataBuffer(buffer), directory(path, EntryType::File)
        {
        }

        FileEntry(io::filtering_istream &in, DirectoryEntry const& pDirectory) : directory(pDirectory)
        {
            this->readFromStream(in);
        }

        void read(std::ifstream &file) // handle exception or file not opened
        {
            std::cout << "reading " << dataSize << "into buffer" << std::endl;
            file.read(dataBuffer.data(), dataSize);
        }

        void writeToStream(io::filtering_ostream &out) override;
        void readFromStream(io::filtering_istream &in) override;
        virtual std::string getPath() const override
        {
            return directory.getPath();
        }
        const char *getData() const override
        {
            return dataBuffer.data();
        }

    private:
        DataBufferType dataBuffer;
        DirectoryEntry directory;
    };

    // struct DataEntry
    // {
    //     void writeToStream(io::filtering_ostream &out);
    //     void readFromStream(io::filtering_istream &in);
    //     std::size_t dataSize;
    //     std::vector<char> dataBuffer;
    // };
} // namespace archive_utils
