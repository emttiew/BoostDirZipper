#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <string>

namespace io = boost::iostreams;
namespace archive_utils
{
    enum class EntryType : char
    {
        Directory = '\x00',
        File = '\x01'
    };

    struct RelativePathEntry
    {
        RelativePathEntry(std::string const &path, EntryType pEntryType) : pathSize(path.size()), filePath(path), entryType(pEntryType) {}
        explicit RelativePathEntry(io::filtering_istream &in)
        {
            this->readFromStream(in);
        }
        void writeToStream(io::filtering_ostream &out);
        void readFromStream(io::filtering_istream &in);

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