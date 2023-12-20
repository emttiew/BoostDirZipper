#pragma once

#include "DataEntry.hpp"
#include "RelativePathEntry.hpp"
#include <memory>

namespace archive_utils
{
    class ArchiveEntry;
    using ArchiveEntryPtr = std::shared_ptr<ArchiveEntry>;

    static ArchiveEntryPtr createEntry(io::filtering_istream &in)
    {
        return std::make_shared<ArchiveEntry>(in);
    }

    class ArchiveEntry
    {
    public:
        explicit ArchiveEntry(io::filtering_istream &in)
        {
            relativePath.readFromStream(in);
            data.readFromStream(in);
        }
        bool isDirectory() const
        {
            return relativePath.isDirectory();
        }
        std::vector<char> getData() const
        {
            return data.getData();
        }
        std::string getFilePath() const
        {
            return relativePath.getFilePath();
        }

    private:
        RelativePathEntry relativePath;
        DataEntry data;
    };
}