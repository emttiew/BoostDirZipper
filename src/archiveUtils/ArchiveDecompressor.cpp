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
        archiveStream.push(io::file_descriptor_source(inputDir.string(), std::ios::binary)); // TODO catch exception if no such directory?
    }

    void ArchiveDecompressor::decompress()
    {
        while (archiveStream.peek() != EOF)
        {
            auto directoryEntry = std::make_shared<DirectoryEntry>();
            directoryEntry->readFromStream(archiveStream);
            if (directoryEntry->isDirectory())
            {
                entries.push_back(std::move(directoryEntry));
                continue;
            }
            auto fileEntry = std::make_shared<FileEntry>();
            fileEntry->readFromStream(archiveStream);
            entries.push_back(std::move(fileEntry));
        }
    }

    EntryPtrVec ArchiveDecompressor::getEntries() const
    {
        return entries;
    }
}