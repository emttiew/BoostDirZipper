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
        FileEntry() = default;
        FileEntry(DataBufferType const &buffer, std::string const &path) : Entry(buffer.size()), dataBuffer(buffer), directory(path, EntryType::File) {}

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
} // namespace archive_utils
