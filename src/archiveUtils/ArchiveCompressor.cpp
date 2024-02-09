#include "ArchiveCompressor.hpp"
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <iostream>
#include <stdexcept>

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
        // TODO check if the file is binary the same to previously added files
        std::ifstream file(filePath.string(), std::ios::binary);
        if (file)
        {
            std::cout << "adding " << filePath.string() << std::endl;
            io::filtering_ostream fileStream;
            fileStream.push(archiveStream);

            FileEntry::DataBufferType buffer(fs::file_size(filePath));
            file.read(buffer.data(), buffer.size());
            if (file.bad())
                throw std::runtime_error("error while reading file " + filePath.string());
            std::string const &path = fs::relative(filePath, inputDir).string();
            FileEntry fileEntry(buffer, path);
            fileEntry.writeToStream(fileStream);
            if (fileStream.bad())
                throw std::runtime_error("error while writing to compressing stream on file " + path);
            file.close();
        }
        else
        {
            std::cout << "error opening file: " << filePath.string() << std::endl;
            throw std::runtime_error("cannot open input file");
        }
    }

    void ArchiveCompressor::addDirectory(const fs::path &filePath)
    {
        io::filtering_ostream fileStream;
        fileStream.push(archiveStream);
        std::string const &path = fs::relative(filePath, inputDir).string();
        DirectoryEntry entry(path, EntryType::Directory);
        entry.writeToStream(fileStream);
        if (fileStream.bad())
            throw std::runtime_error("error while writing to compressing stream on directory " + path);
    }
}