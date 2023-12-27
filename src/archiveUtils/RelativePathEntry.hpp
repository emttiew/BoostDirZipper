#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <string>

namespace io = boost::iostreams;
namespace archive_utils
{
    struct RelativePathEntry
    {
        bool isDirectory() const;
        void writeToStream(io::filtering_ostream &out);
        void readFromStream(io::filtering_istream &in);
        char isRegularFile;
        std::size_t pathSize;
        std::string filePath;
    };
}