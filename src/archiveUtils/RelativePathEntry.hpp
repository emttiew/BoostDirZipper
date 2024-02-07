#pragma once

#include "Entry.hpp"
#include <string>

namespace archive_utils
{
    enum class EntryType : char
    {
        Directory = '\x00',
        File = '\x01'
    };

    struct RelativePathEntry : Entry
    {
        RelativePathEntry(std::string const &path, EntryType pEntryType) : pathSize(path.size()), filePath(path), entryType(pEntryType) {}
        explicit RelativePathEntry(io::filtering_istream &in)
        {
            this->readFromStream(in);
        }
        void writeToStream(io::filtering_ostream &out) override;
        void readFromStream(io::filtering_istream &in) override;

        bool isDirectory() const
        {
            return entryType == EntryType::Directory;
        }

        std::string getPath() const
        {
            return filePath;
        }

    private:
        EntryType entryType;
        std::size_t pathSize;
        std::string filePath;
    };
}