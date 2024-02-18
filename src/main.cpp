#include "archiveUtils/ArchiveUtils.hpp"
#include <filesystem>
#include <iostream>

int main()
{
    namespace fs = std::filesystem;
    const fs::path inputDirectory = "/home/mateusz/Dokumenty/Cpp_projects/compressor_sandbox/BackToTheFutureNew/build/kupa";
    const fs::path compressedFile = "compressed_file.gzip";
    const fs::path decompressedDirectory = "/home/mateusz/Dokumenty/Cpp_projects/compressor_sandbox/BackToTheFutureNew/build/kupa2";
    archive_utils::compressDirectory(inputDirectory, compressedFile);
    archive_utils::decompressDirectory(compressedFile, decompressedDirectory);
}