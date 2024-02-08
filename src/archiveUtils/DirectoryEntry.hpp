#pragma once

#include "Entry.hpp"
#include <string>

namespace archive_utils
{
    class DirectoryEntry : public Entry
    {
    public:
        DirectoryEntry() = default;
        DirectoryEntry(std::string const &path, EntryType pEntryType) : Entry(pEntryType, path.size()), filePath(path) {}

        void writeToStream(io::filtering_ostream &out) override;
        void readFromStream(io::filtering_istream &in) override;
        std::string getPath() const override
        {
            return filePath;
        }
        const char *getData() const override
        {
            return filePath.data();
        }

    private:
        std::string filePath;
    };
}