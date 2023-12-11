#include "Compress.hpp"
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <iostream>

namespace archive_utils
{
    namespace io = boost::iostreams;

    void compressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        io::filtering_ostream out;
        out.push(io::gzip_compressor());
        out.push(io::file_descriptor_sink(outputDir.string()));

        for (fs::recursive_directory_iterator it(inputDir), end; it != end; ++it)
        {
            if (fs::is_regular_file(*it))
            {
                fs::path relativePath = fs::relative(it->path(), inputDir);
                std::cout << "relativePath: " << relativePath.string() << std::endl;
                // out << relativePath.string() << '\n';

                std::fstream file;
                file.open(it->path().string(), std::ios::out);
                if (file)
                {
                    io::copy(file, out);
                    file.close();
                }
                else
                    std::cout << "error opening file: " << relativePath.string() << std::endl;
            }
        }
    }

    void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir)
    {
        io::filtering_istream in;
        in.push(io::gzip_decompressor());
        in.push(io::file_descriptor_source(inputDir.string()));

        int i = 1;
        while (in.peek() != EOF)
        {
            std::cout << "start " << i << std::endl;
            std::string relativePath;
            std::getline(in, relativePath);

            std::cout << "relativePath: " << relativePath << std::endl;

            if (relativePath.empty())
            {
                break; // End of compressed data
            }

            // fs::path outputPath = outputDir / relativePath;
            // std::cout << "outputPath: " << outputPath.string() << std::endl;
            // fs::create_directories(outputPath.parent_path());
            // std::cout << "good" << std::endl;

            // std::ofstream file(outputPath.string(), std::ios_base::binary);
            // std::cout << "good2" << std::endl;
            // io::copy(in, file);
            // std::cout << "good3" << std::endl;
            ++i;
        }
        std::cout << "end" << std::endl;
    }

    void hello()
    {
        std::cout << "Hello" << std::endl;
    }
}