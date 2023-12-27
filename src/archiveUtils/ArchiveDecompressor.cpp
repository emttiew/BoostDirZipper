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

    void ArchiveDecompressor::decompress()
    {
        while (archiveStream.peek() != EOF)
        {
            auto entry = std::make_shared<ArchiveEntry>();
            entry->read(archiveStream);
            entries.push_back(std::move(entry));
        }
    }

    ArchiveEntryPtrVec ArchiveDecompressor::getEntries() const
    {
        return entries;
    }
}