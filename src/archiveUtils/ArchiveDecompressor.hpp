#pragma once

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <memory>
#include <vector>

#include "Entry.hpp"

namespace fs = boost::filesystem;
namespace io = boost::iostreams;

namespace archive_utils
{
    using EntryPtr = std::shared_ptr<Entry>;
    using EntryPtrVec = std::vector<EntryPtr>;

    class ArchiveDecompressor
    {
    public:
        explicit ArchiveDecompressor(const fs::path &inputDir);
        EntryPtrVec getEntries() const;
        void decompress(); // should throw, return error code ...

    private:
        io::filtering_istream archiveStream;
        EntryPtrVec entries;
    };
}