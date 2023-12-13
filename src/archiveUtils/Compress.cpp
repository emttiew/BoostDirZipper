#include "Compress.hpp"
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <iostream>
#include <zlib.h>

namespace archive_utils
{
    namespace io = boost::iostreams;

    void compressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        io::filtering_ostream out;
        out.push(io::gzip_compressor());
        out.push(io::file_sink(outputDir.string(), std::ios::binary));

        for (fs::recursive_directory_iterator it(inputDir), end; it != end; ++it)
        {
            io::filtering_ostream outStreamFile;
            outStreamFile.push(out);
            if (fs::is_regular_file(*it))
            {
                std::cout << "file path: " << it->path().string() << std::endl;
                std::ifstream file(it->path().string(), std::ios::binary);
                if (file)
                {
                    outStreamFile.put('\x00');
                    fs::path relativePath = fs::relative(it->path(), inputDir);
                    std::size_t relativePathSize = relativePath.string().size();
                    std::cout << "compression relativePathSize: " << relativePathSize << std::endl;
                    std::cout << "relativePath: " << relativePath.string() << std::endl;
                    outStreamFile.write(reinterpret_cast<char *>(&relativePathSize), sizeof(std::size_t));
                    outStreamFile.write(relativePath.c_str(), relativePathSize);
                    std::uintmax_t dataSize = fs::file_size(it->path());
                    std::cout << "dataSize: " << dataSize << std::endl;
                    outStreamFile.write(reinterpret_cast<char *>(&dataSize), sizeof(std::uintmax_t));
                    std::size_t bytes_copied = io::copy(file, outStreamFile);
                    std::cout << "Compression: Number of bytes copied: " << bytes_copied << std::endl;
                    file.close();
                }
                else
                    std::cout << "error opening file: " << it->path().string() << std::endl;
            }
            if (fs::is_directory(*it))
            {
                outStreamFile.put('\x01');
                fs::path relativePath = fs::relative(it->path(), inputDir);
                std::size_t relativePathSize = relativePath.string().size();
                outStreamFile.write(reinterpret_cast<char *>(&relativePathSize), sizeof(std::size_t));
                outStreamFile.write(relativePath.c_str(), relativePathSize);
                std::cout << "dirctory path: " << it->path().string() << std::endl;
            }
        }

        std::cout << "out size: " << out.size() << std::endl;
        out.reset();
    }

    namespace
    {
        bool isDirectory(io::filtering_istream &stream)
        {
            char value;
            stream.get(value);
            return value != '\x00'; // Returns true if value is not zero
        }

        void ensureDirectoryExists(fs::path const &dir)
        {
            if (!fs::exists(dir))
                fs::create_directories(dir);
        }
    }

    void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        io::filtering_istream in;
        in.push(io::gzip_decompressor());
        in.push(io::file_descriptor_source(inputDir.string(), std::ios::binary));
        ensureDirectoryExists(outputDir);

        while (in.peek() != EOF)
        {
            bool isRegularFile = !isDirectory(in);
            size_t relativePathSize = 0;
            in.read(reinterpret_cast<char *>(&relativePathSize), sizeof(size_t));
            std::cout << "relativePathSize: " << relativePathSize << std::endl;
            std::vector<char> pathBuffer(relativePathSize);
            in.read(pathBuffer.data(), relativePathSize);
            std::string relativePath(pathBuffer.data(), relativePathSize);
            std::cout << "decom relativePath: " << relativePath << std::endl;
            auto filepath = outputDir / fs::path{relativePath}.relative_path();
            std::cout << "filepath: " << filepath.string() << std::endl;
            std::cout << "filepath parent: " << filepath.parent_path().string() << std::endl;

            if (!isRegularFile)
            {
                ensureDirectoryExists(filepath);
            }
            else
            {
                ensureDirectoryExists(filepath.parent_path());
                std::ofstream destFile;
                destFile.open(filepath.string().c_str(), std::ios::binary | std::ios::trunc);
                std::uintmax_t dataSize = 0;
                in.read(reinterpret_cast<char *>(&dataSize), sizeof(std::uintmax_t));
                std::cout << "decom dataSize: " << dataSize << std::endl;
                std::vector<char> dataBuffer(dataSize);
                in.read(dataBuffer.data(), dataSize);
                destFile.write(dataBuffer.data(), dataBuffer.size());
            }
        }
    }
}