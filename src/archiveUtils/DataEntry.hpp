#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <vector>

namespace archive_utils
{
    namespace io = boost::iostreams;

    class DataEntry
    {
    public:
        DataEntry() = default;
        explicit DataEntry(std::size_t pDataSize, std::ifstream &file);
        void writeToStream(io::filtering_ostream &out);
        void readFromStream(io::filtering_istream &in);
        std::vector<char> getData() const;
        std::size_t getDataSize() const;

    private:
        std::size_t dataSize;
        std::vector<char> dataBuffer;
    };
} // namespace archive_utils