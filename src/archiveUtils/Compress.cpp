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
            if (fs::is_regular_file(*it))
            {
                std::cout << "file path: " << it->path().string() << std::endl;
                std::ifstream file(it->path().string(), std::ios::binary);
                if (file)
                {
                    fs::path relativePath = fs::relative(it->path(), inputDir);
                    size_t relativePathSize = relativePath.string().size();
                    std::cout << "compression relativePathSize: " << relativePathSize << std::endl;
                    std::cout << "relativePath: " << relativePath.string() << std::endl;
                    out.write(reinterpret_cast<char *>(&relativePathSize), sizeof(size_t));
                    out.write(relativePath.c_str(), relativePathSize);
                    std::size_t bytes_copied = io::copy(file, out);
                    // out.write(reinterpret_cast<char *>(&bytes_copied), sizeof(size_t));
                    std::cout << "Compression: Number of bytes copied: " << bytes_copied << " size of streamsize " << sizeof(std::size_t) << std::endl;
                    file.close();
                }
                else
                    std::cout << "error opening file: " << it->path().string() << std::endl;
            }
            if (fs::is_directory(*it))
                std::cout << "dirctory path: " << it->path().string() << std::endl;
        }

        std::cout << "out size: " << out.size() << std::endl;
        out.reset();

        //         fs::path relativePath = fs::relative(it->path(), inputDir);
        //         std::cout << "relativePath: " << relativePath.string() << std::endl;
        //         out << relativePath.string() << '\n';
    }

    void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        io::filtering_istream in;
        in.push(io::gzip_decompressor());
        in.push(io::file_descriptor_source(inputDir.string(), std::ios::binary));
        std::fstream out;
        out.open(outputDir.string(), std::ios::out | std::ios::binary);

        if (out)
        {
            size_t relativePathSize = 0;
            in.read(reinterpret_cast<char *>(&relativePathSize), sizeof(size_t));
            std::cout << "relativePathSize: " << relativePathSize << std::endl;
            std::vector<char> pathBuffer(relativePathSize);
            in.read(pathBuffer.data(), relativePathSize);
            std::string relativePath(pathBuffer.data(), relativePathSize);
            std::cout << "decom relativePath: " << relativePath << std::endl;

            std::streamsize bytes_copied = io::copy(in, out);
            std::cout << "Decompression: Number of bytes copied: " << bytes_copied << std::endl;
        }
        else
        {
            std::cout << "Decompression: Error opening file" << std::endl;
        }

        // int i = 1;
        // while (in.peek() != EOF)
        // {
        //     std::cout << "start " << i << std::endl;
        //     std::string relativePath;
        //     std::getline(in, relativePath);

        //     std::cout << "relativePath: " << relativePath << std::endl;

        //     if (relativePath.empty())
        //     {
        //         break; // End of compressed data
        //     }

        //     // fs::path outputPath = outputDir / relativePath;
        //     // std::cout << "outputPath: " << outputPath.string() << std::endl;
        //     // fs::create_directories(outputPath.parent_path());
        //     // std::cout << "good" << std::endl;

        //     // std::ofstream file(outputPath.string(), std::ios_base::binary);
        //     // std::cout << "good2" << std::endl;
        //     // io::copy(in, file);
        //     // std::cout << "good3" << std::endl;
        //     ++i;
        // }
        // std::cout << "end" << std::endl;
    }

    void hello()
    {
        std::cout << "Hello" << std::endl;
    }
}