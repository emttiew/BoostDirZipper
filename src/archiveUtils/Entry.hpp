#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <string>

namespace archive_utils
{
    namespace io = boost::iostreams;

    enum class EntryType : char
    {
        Directory = '\x00',
        File = '\x01'
    };

    class Entry
    {
    public:
        Entry() = default;
        virtual void writeToStream(io::filtering_ostream &out) = 0;
        virtual void readFromStream(io::filtering_istream &in) = 0;
        virtual std::string getPath() const = 0;
        virtual const char *getData() const = 0;

        size_t getDataSize() const { return dataSize; }
        bool isDirectory() const
        {
            return entryType == EntryType::Directory;
        }
        virtual ~Entry() {}
        
    protected:
        explicit Entry(EntryType pEntryType) : entryType(pEntryType) {}
        explicit Entry(size_t pDataSize) : dataSize(pDataSize) {}
        explicit Entry(EntryType pEntryType, size_t pDataSize) : entryType(pEntryType), dataSize(pDataSize) {}
        EntryType entryType;
        size_t dataSize = 0;
    };
}