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

target_link_libraries(JsonLib_UnitTests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE FilesystemLib
    PRIVATE Kmplete
)

SetupUnitTestsTargetProperties(JsonLib_UnitTests Kmplete/JsonLib/Tests)
