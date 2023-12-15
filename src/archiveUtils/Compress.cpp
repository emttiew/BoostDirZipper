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

    struct RelativePathEntry
    {
        RelativePathEntry() = default;
        explicit RelativePathEntry(char pIsRegularFile, std::size_t pPathSize, const std::string &pFilePath)
            : isRegularFile(pIsRegularFile), pathSize(pPathSize), filePath(pFilePath)
        {
        }

        bool isDirectory() const
        {
            return isRegularFile == '\x00';
        }

        void writeToStream(io::filtering_ostream &out)
        {
            out.put(isRegularFile);
            out.write(reinterpret_cast<char *>(&pathSize), sizeof(pathSize));
            out.write(filePath.data(), pathSize);
        }

    private:
        char isRegularFile;
        std::size_t pathSize;
        std::string filePath;
    };

    struct DataEntry
    {
        DataEntry() = default;
        explicit DataEntry(std::size_t pDataSize) : dataSize(pDataSize)
        {
            dataBuffer.resize(dataSize);
        }

        void writeToStream(io::filtering_ostream &out)
        {
            out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
            out.write(dataBuffer.data(), dataSize);
        }

    private:
        std::size_t dataSize;
        std::vector<char> dataBuffer;
    };

    class Archive
    {
    public:
        Archive() = default;
        explicit Archive(const fs::path &filePath)
        {
            archiveStream.push(io::gzip_compressor());
            archiveStream.push(io::file_sink(filePath.string(), std::ios::binary));
        }

        // ~Archive()
        // {
        //     archiveStream.reset();
        // }

        void addFile(const fs::path &filePath)
        {
            // open file (TODO add exception safety!)
            std::ifstream file(filePath.string(), std::ios::binary);
            if (file)
            {
                fileStream.push(archiveStream);
                RelativePathEntry relativePath('\x01', filePath.string().size(), filePath.string());
                DataEntry dataEntry(fs::file_size(filePath));
                relativePath.writeToStream(fileStream);
                dataEntry.writeToStream(fileStream);
                file.close();
            }
            else
            {
                std::cout << "error opening file: " << filePath.string() << std::endl;
            }
        }

        void addDirectory(const fs::path &filePath)
        {
            fileStream.push(archiveStream);
            RelativePathEntry entry('\x00', filePath.string().size(), filePath.string());
            entry.writeToStream(fileStream);
        }

    private:
        // TODO figrue out good way to defince constructors etc.
        RelativePathEntry relativePath;
        DataEntry data;
        io::filtering_ostream archiveStream;
        io::filtering_ostream fileStream;
    };

    struct PacketHeader
    {
        char isRegularFile;
        std::size_t filePathSize;
        std::string filePath;
        std::size_t dataSize;
        std::vector<char> dataBuffer;

        bool isDirectory() const
        {
            return isRegularFile == '\x00';
        }
    };

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
                    outStreamFile.put('\x01');
                    PacketHeader packetHeader;
                    packetHeader.filePath = fs::relative(it->path(), inputDir).string();
                    packetHeader.filePathSize = packetHeader.filePath.size();
                    std::cout << "compression relativePathSize: " << packetHeader.filePathSize << std::endl;
                    std::cout << "relativePath: " << packetHeader.filePath << std::endl;
                    outStreamFile.write(reinterpret_cast<char *>(&packetHeader.filePathSize), sizeof(std::size_t));
                    outStreamFile.write(packetHeader.filePath.data(), packetHeader.filePathSize);
                    packetHeader.dataSize = fs::file_size(it->path());
                    std::cout << "dataSize: " << packetHeader.dataSize << std::endl;
                    outStreamFile.write(reinterpret_cast<char *>(&packetHeader.dataSize), sizeof(packetHeader.dataSize));
                    std::size_t bytes_copied = io::copy(file, outStreamFile);
                    std::cout << "Compression: Number of bytes copied: " << bytes_copied << std::endl;
                    file.close();
                }
                else
                    std::cout << "error opening file: " << it->path().string() << std::endl;
            }
            if (fs::is_directory(*it))
            {
                outStreamFile.put('\x00');
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
            PacketHeader packetHeader;
            in.get(packetHeader.isRegularFile);
            in.read(reinterpret_cast<char *>(&packetHeader.filePathSize), sizeof(packetHeader.filePathSize));
            std::cout << "relativePathSize: " << packetHeader.filePathSize << std::endl;
            packetHeader.filePath.resize(packetHeader.filePathSize);
            in.read(&packetHeader.filePath[0], packetHeader.filePathSize);
            std::cout << "decom relativePath: " << packetHeader.filePath << std::endl;
            auto filepath = outputDir / fs::path{packetHeader.filePath}.relative_path();
            std::cout << "filepath: " << filepath.string() << std::endl;
            std::cout << "filepath parent: " << filepath.parent_path().string() << std::endl;

            if (packetHeader.isDirectory())
            {
                ensureDirectoryExists(filepath);
            }
            else
            {
                ensureDirectoryExists(filepath.parent_path());
                std::ofstream destFile;
                destFile.open(filepath.string().c_str(), std::ios::binary | std::ios::trunc);

                in.read(reinterpret_cast<char *>(&packetHeader.dataSize), sizeof(packetHeader.dataSize));
                std::cout << "decom dataSize: " << packetHeader.dataSize << std::endl;
                packetHeader.dataBuffer.resize(packetHeader.dataSize);
                in.read(packetHeader.dataBuffer.data(), packetHeader.dataSize);
                destFile.write(packetHeader.dataBuffer.data(), packetHeader.dataBuffer.size());
            }
        }
    }
}