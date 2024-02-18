#pragma once

#include <filesystem>

namespace archive_utils {
namespace fs = std::filesystem;

void compressDirectory(const fs::path &inputDir, const fs::path &outputDir);
void decompressDirectory(const fs::path &inputDir, const fs::path &outputDir);
}  // namespace archive_utils