## ~/src/FilesystemLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(FilesystemLib_UnitTests_FILESYSTEM
    ${CMAKE_CURRENT_LIST_DIR}/filesystem_tests.cpp
)

add_executable(FilesystemLib_UnitTests
    ${FilesystemLib_UnitTests_FILESYSTEM}
)

target_link_libraries(FilesystemLib_UnitTests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE FilesystemLib
    PRIVATE Kmplete
)

SetupUnitTestsTargetProperties(FilesystemLib_UnitTests Kmplete/FilesystemLib/Tests)
