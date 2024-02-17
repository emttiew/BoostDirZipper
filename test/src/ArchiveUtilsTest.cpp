#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <sstream>
#include <ArchiveUtils.hpp>

using namespace ::testing;
namespace fs = boost::filesystem;

class ArchiveUtilsTest : public Test {
protected:
    void SetUp() override {
        // Create a temporary directory for testing
        tempDir = fs::temp_directory_path() / "archive_test";
        fs::create_directory(tempDir);

        // Create some test files in the temporary directory
        std::ofstream file1(tempDir / "file1.txt");
        file1 << "Test data for file1";
        file1.close();

        fs::create_directory(tempDir / "subdir");
        std::ofstream file2(tempDir / "subdir" / "file2.txt");
        file2 << "Test data for file2";
        file2.close();
    }

    void TearDown() override {
        fs::remove_all(tempDir);
    }

    fs::path tempDir;
};

TEST_F(ArchiveUtilsTest, decompressed_data_is_equal_to_original_data) {
    fs::path inputDir = tempDir;
    fs::path compressedFile = "compressed_file.gzip";

    std::ostringstream originalContents;
    for (fs::recursive_directory_iterator it(inputDir), end; it != end; ++it) {
        if (fs::is_regular_file(*it)) {
            std::ifstream file(it->path());
            originalContents << file.rdbuf();
            file.close();
        }
    }

    archive_utils::compressDirectory(inputDir, compressedFile);

    fs::path decompressedDir = tempDir / "decompressed";
    archive_utils::decompressDirectory(compressedFile, decompressedDir);

    std::ostringstream decompressedContents;
    for (fs::recursive_directory_iterator it(decompressedDir), end; it != end; ++it) {
        if (fs::is_regular_file(*it)) {
            std::ifstream file(it->path());
            decompressedContents << file.rdbuf();
            file.close();
        }
    }

    ASSERT_EQ(originalContents.str(), decompressedContents.str());
}

TEST_F(ArchiveUtilsTest, throw_when_noexistent_dir) {
    fs::path inputDir = tempDir / "nonexistent_dir";
    fs::path outputDir = tempDir / "compressed";

    ASSERT_THROW(archive_utils::compressDirectory(inputDir, outputDir), std::runtime_error);
}

TEST_F(ArchiveUtilsTest, throw_when_provided_file_instead_of_folder) {
    std::ofstream file(tempDir / "single_file.txt");
    file << "Test data for single file";
    file.close();

    fs::path inputDir = tempDir / "single_file.txt";
    fs::path outputDir = tempDir / "compressed";

    ASSERT_THROW(archive_utils::compressDirectory(inputDir, outputDir), std::runtime_error);
}
