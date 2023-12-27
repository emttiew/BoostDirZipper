#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <vector>

namespace archive_utils
{
    namespace io = boost::iostreams;

    struct DataEntry
    {
        void writeToStream(io::filtering_ostream &out);
        void readFromStream(io::filtering_istream &in);
        std::size_t dataSize;
        std::vector<char> dataBuffer;
    };
} // namespace archive_utils

// TODO stop using vector

// #include <memory>
// #include <cstring> // For std::memcpy

// class DataBuffer {
// public:
//     // User-defined constructor
//     DataBuffer(std::size_t dataSize) : dataSize(dataSize) {
//         dataBuffer = std::make_unique<char[]>(dataSize);
//     }

//     // Function to set data into the buffer
//     void setData(const char* newData) {
//         std::memcpy(dataBuffer.get(), newData, dataSize);
//     }

//     // Function to retrieve the data
//     const char* getData() const {
//         return dataBuffer.get();
//     }

// private:
//     std::size_t dataSize;
//     std::unique_ptr<char[]> dataBuffer;
// };