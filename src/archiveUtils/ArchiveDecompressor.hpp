#pragma once

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <memory>
#include <vector>

#include "ArchiveEntry.hpp"

namespace fs = boost::filesystem;
namespace io = boost::iostreams;

namespace archive_utils
{
    using ArchiveEntryPtr = std::shared_ptr<ArchiveEntry>;
    using ArchiveEntryPtrVec = std::vector<ArchiveEntryPtr>;

    class ArchiveDecompressor
    {
    public:
        explicit ArchiveDecompressor(const fs::path &inputDir);
        ArchiveEntryPtrVec getEntries() const;
        void decompress(); // should throw, return error code ...
        
    private:
        io::filtering_istream archiveStream;
        ArchiveEntryPtrVec entries;
    };
}