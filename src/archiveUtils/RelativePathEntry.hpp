#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <string>

namespace io = boost::iostreams;
namespace archive_utils
{
    class RelativePathEntry
    {
    public:
        RelativePathEntry() = default;
        explicit RelativePathEntry(char pIsRegularFile, const std::string &pFilePath);
        bool isDirectory() const;
        void writeToStream(io::filtering_ostream &out);
        void readFromStream(io::filtering_istream &in);
        std::string getFilePath() const;

    private:
        char isRegularFile;
        std::size_t pathSize;
        std::string filePath;
    };
}