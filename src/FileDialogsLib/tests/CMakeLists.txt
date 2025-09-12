## ~/src/FileDialogsLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(FileDialogsLib_UnitTests_FILEDIALOGS
    ${CMAKE_CURRENT_LIST_DIR}/file_dialogs_tests.cpp
)

add_executable(FileDialogsLib_UnitTests
    ${FileDialogsLib_UnitTests_FILEDIALOGS}
)

SetupUnitTestsTargetProperties(
    FileDialogsLib_UnitTests 
    Kmplete/Tests
    Catch2::Catch2WithMain FileDialogsLib Kmplete
)