#pragma once

#include <boost/filesystem.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "DataEntry.hpp"
#include "RelativePathEntry.hpp"

namespace fs = boost::filesystem;

namespace archive_utils {
class ArchiveCompressor
{
public:
    ArchiveCompressor(const fs::path &outputDir, const fs::path &pInputDir);
    void addFile(const fs::path &filePath);
    void addDirectory(const fs::path &filePath);

    // ~Archive()
    // {
    //     archiveStream.reset();
    // }
private:
    // TODO figrue out good way to defince constructors etc.
    io::filtering_ostream archiveStream;
    fs::path inputDir;
};
}
