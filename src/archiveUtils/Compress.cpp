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
        explicit RelativePathEntry(char pIsRegularFile, const std::string &pFilePath)
            : isRegularFile(pIsRegularFile), pathSize(pFilePath.size()), filePath(pFilePath)
        {
        }

        bool isDirectory() const
        {
            return isRegularFile == '\x00';
        }

        void writeToStream(io::filtering_ostream &out)
        {
            std::cout << "isRegularFile: " << isRegularFile << std::endl;
            std::cout << "pathSize: " << pathSize << std::endl;
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
            std::cout << "dataSize: " << dataSize << std::endl;
            out.write(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
            out.write(dataBuffer.data(), dataSize);
        }

        void readDataFromFile(std::ifstream &file)
        {
            if (file)
                file.read(dataBuffer.data(), dataSize); // handle bad bit
            // throw or something
        }

    private:
        std::size_t dataSize;
        std::vector<char> dataBuffer;
    };

    class Archive
    {
    public:
        Archive() = default;
        explicit Archive(const fs::path &outputDir, const fs::path &pInputDir) : inputDir(pInputDir)
        {
            archiveStream.push(io::gzip_compressor());
            archiveStream.push(io::file_sink(outputDir.string(), std::ios::binary));
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
                io::filtering_ostream fileStream;
                fileStream.push(archiveStream);
                RelativePathEntry relativePathEntry('\x01', fs::relative(filePath, inputDir).string());
                DataEntry dataEntry(fs::file_size(filePath));
                relativePathEntry.writeToStream(fileStream);
                dataEntry.readDataFromFile(file);
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
            io::filtering_ostream fileStream;
            fileStream.push(archiveStream);
            RelativePathEntry entry('\x00', fs::relative(filePath, inputDir).string());
            entry.writeToStream(fileStream);
        }

    private:
        // TODO figrue out good way to defince constructors etc.
        RelativePathEntry relativePath;
        DataEntry data;
        io::filtering_ostream archiveStream;
        fs::path inputDir;
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
        Archive archive(outputDir, inputDir);

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