cmake_minimum_required(VERSION 3.16)
project(ArchiveUtils CXX)

set(SOURCE_FILES
    Compress.cpp
    Decompress.cpp
)

add_library(ArchiveUtils STATIC ${SOURCE_FILES})
find_package(Boost 1.71.0 COMPONENTS filesystem iostreams) 

if(Boost_FOUND)
    target_link_libraries(ArchiveUtils ${Boost_LIBRARIES})
else()
  message(FATAL_ERROR "Boost library not found")
endif()

install(TARGETS ArchiveUtils DESTINATION ${LIB_DIR})
install(FILES Decompress.hpp Compress.hpp DESTINATION ${INCLUDE_DIR})