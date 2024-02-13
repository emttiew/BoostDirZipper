#pragma once

#include <boost/filesystem.hpp>

namespace archive_utils {
namespace fs = boost::filesystem;

void compressDirectory(const fs::path &inputDir, const fs::path &outputDir);
void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir);
}  // namespace archive_utils