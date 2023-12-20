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
    using ArchiveEntryPtrVec = std::vector<ArchiveEntryPtr>;
    class ArchiveDecompressor
    {
    public:
        ArchiveDecompressor() = default;
        explicit ArchiveDecompressor(const fs::path &inputDir);
        ArchiveEntryPtrVec getEntries();

        // ~ArchiveDecompressor()
        // {
        //     archiveStream.reset();
        // }

    private:
        io::filtering_istream archiveStream;
    };
}