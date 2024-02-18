include(FetchContent)

set(TEST_NAME ${PROJECT_NAME}-Test)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES test.cpp src/ArchiveUtilsTest.cpp)

add_executable(${TEST_NAME} ${SOURCE_FILES})
target_link_libraries(${TEST_NAME} gtest_main ArchiveUtils)
set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
