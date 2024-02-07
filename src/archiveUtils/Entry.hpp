#pragma once

#include <boost/iostreams/filtering_stream.hpp>

namespace archive_utils
{
    namespace io = boost::iostreams;
    class Entry
    {
    public:
        virtual void writeToStream(io::filtering_ostream &out) = 0;
        virtual void readFromStream(io::filtering_istream &in) = 0;
        virtual ~Entry() {}
    };
}