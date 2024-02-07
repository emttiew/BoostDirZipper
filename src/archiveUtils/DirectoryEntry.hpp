#pragma once

#include "Entry.hpp"
#include <string>

namespace archive_utils
{

    struct DirectoryEntry : Entry
    {
        DirectoryEntry(std::string const &path, EntryType pEntryType) : Entry(pEntryType), pathSize(path.size()), filePath(path) {}
        explicit DirectoryEntry(io::filtering_istream &in)
        {
            this->readFromStream(in);
        }
        void writeToStream(io::filtering_ostream &out) override;
        void readFromStream(io::filtering_istream &in) override;

        std::string getPath() const
        {
            return filePath;
        }

    private:
        std::size_t pathSize;
        std::string filePath;
    };
}