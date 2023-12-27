#pragma once

#include "DataEntry.hpp"
#include "RelativePathEntry.hpp"
#include <memory>

namespace archive_utils
{
    class ArchiveEntry
    {
    public:
        void read(io::filtering_istream &in) // should return error code, handle bad bit or handle exceptions
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
            return data.dataBuffer;
        }
        std::string getFilePath() const
        {
            return relativePath.filePath;
        }

    private:
        RelativePathEntry relativePath;
        DataEntry data;
    };
}