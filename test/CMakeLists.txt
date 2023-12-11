cmake_minimum_required(VERSION 3.16)
project(BoostDirZipperTests)

add_subdirectory(googleTest)

include_directories(${HEADERS_DIR})
include_directories(googleTest/googletest/googletest/include)

set(SOURCE_FILES main.cpp src/DecompressTest.cpp)

add_executable(BoostDirZipperTests ${SOURCE_FILES})
target_link_libraries(BoostDirZipperTests ArchiveUtils gtest)
install(TARGETS BoostDirZipperTests DESTINATION ${BIN_DIR})
