#include "ArchiveDecompressor.hpp"

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace archive_utils
{

    ArchiveDecompressor::ArchiveDecompressor(const fs::path &inputDir)
    {
        archiveStream.push(io::gzip_decompressor());
        archiveStream.push(io::file_descriptor_source(inputDir.string(), std::ios::binary));
    }

    ArchiveEntryPtrVec ArchiveDecompressor::getEntries()
    {
        ArchiveEntryPtrVec entries;
        while (archiveStream.peek() != EOF)
        {
            entries.push_back(createEntry(archiveStream));
        }
        return entries;
    }
}