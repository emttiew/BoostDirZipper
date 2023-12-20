#include "Compress.hpp"
#include "ArchiveCompressor.hpp"
#include "ArchiveDecompressor.hpp"

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <iostream>
#include <zlib.h>

namespace archive_utils
{
    namespace io = boost::iostreams;

    // TODO this function could be inside archive class
    void compressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        ArchiveCompressor archive(outputDir, inputDir);

        // TODO what if inputDir is not a directory
        for (fs::recursive_directory_iterator it(inputDir), end; it != end; ++it)
        {
            if (fs::is_regular_file(*it))
            {
                std::cout << "file path: " << it->path().string() << std::endl;
                archive.addFile(it->path());
            }
            if (fs::is_directory(*it))
            {
                std::cout << "dirctory path: " << it->path().string() << std::endl;
                archive.addDirectory(it->path());
            }
        }
    }

    namespace
    {
        void ensureDirectoryExists(fs::path const &dir)
        {
            if (!fs::exists(dir))
                fs::create_directories(dir);
        }
    }

    void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        ArchiveDecompressor archive(inputDir);
        ensureDirectoryExists(outputDir);

        for (auto const& entry : archive.getEntries())
        {
            if (entry)
            {
                auto filepath = outputDir / fs::path{entry->getFilePath()}.relative_path();
                if (entry->isDirectory())
                {
                    ensureDirectoryExists(filepath);
                }
                else
                {
                    ensureDirectoryExists(filepath.parent_path());
                    std::ofstream destFile;
                    destFile.open(filepath.string().c_str(), std::ios::binary | std::ios::trunc);
                    auto const &data = entry->getData();
                    destFile.write(data.data(), data.size());
                    destFile.close();
                }
            }
        }
    }
}