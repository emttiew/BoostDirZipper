#pragma once

#include "DataEntry.hpp"
#include "RelativePathEntry.hpp"
#include <memory>

namespace archive_utils
{
    class ArchiveEntry
    {
    public:
        ArchiveEntry(io::filtering_istream &in) : archiveStream(in) ,relativePath(in), data(in) {}
        // void read() // TODO: should return error code, handle bad bit or handle exceptions
        // {
        //     relativePath.readFromStream(archiveStream);
        //     data.readFromStream(archiveStream);
        // }

        void write(std::ofstream &file) const // TODO: exception hadnle
        {
            file.write(data.getData(), data.getSize());
        }

        bool isDirectory() const
        {
            return relativePath.isDirectory();
        }

        std::string getFilePath() const
        {
            return relativePath.getPath();
        }

    private:
        io::filtering_istream& archiveStream;
        RelativePathEntry relativePath;
        DataEntry data;
    };
}