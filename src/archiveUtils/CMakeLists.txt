cmake_minimum_required(VERSION 3.16)
project(ArchiveUtils CXX)

set(SOURCE_FILES
    ArchiveCompressor.cpp
    ArchiveDecompressor.cpp
    ArchiveUtils.cpp
    DirectoryEntry.cpp
    FileEntry.cpp
)

add_library(ArchiveUtils STATIC ${SOURCE_FILES})
find_package(Boost 1.71.0 COMPONENTS iostreams) 

if(Boost_FOUND)
    target_link_libraries(ArchiveUtils ${Boost_LIBRARIES})
else()
  message(FATAL_ERROR "Boost library not found")
endif()

install(TARGETS ArchiveUtils DESTINATION ${LIB_DIR})
install(FILES ArchiveUtils.hpp DESTINATION ${INCLUDE_DIR})