#include "ArchiveDecompressor.hpp"
#include "DirectoryEntry.hpp"
#include "FileEntry.hpp"

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
            auto directoryEntry = std::make_shared<DirectoryEntry>(archiveStream);
            if (directoryEntry->isDirectory())
            {
                entries.push_back(std::move(directoryEntry));
                continue;
            }

            entries.push_back(std::make_shared<FileEntry>(archiveStream, *directoryEntry));
        }
    }

    ArchiveEntryPtrVec ArchiveDecompressor::getEntries() const
    {
        return entries;
    }
}