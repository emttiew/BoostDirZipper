#pragma once

#include <boost/iostreams/filtering_stream.hpp>

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
        Entry() = default; // deafult for now - later consider extra EntryType for decompressing
        virtual void writeToStream(io::filtering_ostream &out) = 0;
        virtual void readFromStream(io::filtering_istream &in) = 0;

        bool isDirectory() const
        {
            return entryType == EntryType::Directory;
        }

        virtual ~Entry() {}
        
    protected:
        explicit Entry(EntryType pEntryType) : entryType(pEntryType) {}
        EntryType entryType;
    };
}