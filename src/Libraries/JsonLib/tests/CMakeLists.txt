## ~/src/JsonLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(JsonLib_UnitTests_JSON
    ${CMAKE_CURRENT_LIST_DIR}/json_document_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/json_reader_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/json_writer_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/json_reader_writer_tests.cpp
)

add_executable(JsonLib_UnitTests
    ${JsonLib_UnitTests_JSON}
)

SetupUnitTestsTargetProperties(
    JsonLib_UnitTests 
    Kmplete/Tests
    Catch2::Catch2WithMain FilesystemLib Kmplete
)
