cmake_minimum_required(VERSION 3.16)

project(BoostDirZipper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -Wall -Werror -Wpedantic -Wextra")

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/build/bin)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/build/lib)

set(HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/archiveUtils)

include_directories(${INCLUDE_DIR})
include_directories(${HEADERS_DIR})

add_subdirectory(src)
add_subdirectory(test)