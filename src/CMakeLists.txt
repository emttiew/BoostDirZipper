add_subdirectory(archiveUtils)

add_executable(BoostDirZipper main.cpp)

target_link_libraries(BoostDirZipper ArchiveUtils)

install(TARGETS BoostDirZipper DESTINATION ${BIN_DIR})
