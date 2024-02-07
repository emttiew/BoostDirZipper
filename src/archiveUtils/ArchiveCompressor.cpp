#include "ArchiveCompressor.hpp"
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <iostream>

namespace fs = boost::filesystem;
namespace io = boost::iostreams;

namespace archive_utils
{
    ArchiveCompressor::ArchiveCompressor(const fs::path &outputDir, const fs::path &pInputDir) : inputDir(pInputDir)
    {
        archiveStream.push(io::gzip_compressor());
        archiveStream.push(io::file_descriptor_sink(outputDir.string(), std::ios::binary));
    }

    void ArchiveCompressor::addFile(const fs::path &filePath)
    {
        // open file (TODO add exception safety!)
        // TODO check if the file is binary the same to previously added files
        std::ifstream file(filePath.string(), std::ios::binary);
        if (file)
        {
            std::cout << "adding " << filePath.string() << std::endl;
            io::filtering_ostream fileStream;
            fileStream.push(archiveStream);
            std::string const &path = fs::relative(filePath, inputDir).string();
            DirectoryEntry relativePathEntry(path, EntryType::File);
            FileEntry dataEntry(fs::file_size(filePath));
            dataEntry.read(file);
            relativePathEntry.writeToStream(fileStream);
            dataEntry.writeToStream(fileStream);
            file.close();
        }
        else
        {
            std::cout << "error opening file: " << filePath.string() << std::endl;
        }
    }

    void ArchiveCompressor::addDirectory(const fs::path &filePath)
    {
        io::filtering_ostream fileStream;
        fileStream.push(archiveStream);
        std::string const &path = fs::relative(filePath, inputDir).string();
        DirectoryEntry entry(path, EntryType::Directory);
        entry.writeToStream(fileStream);
    }
}