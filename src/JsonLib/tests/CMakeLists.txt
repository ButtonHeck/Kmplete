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

SetupCompilerOptions(JsonLib_UnitTests)

target_link_libraries(JsonLib_UnitTests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE FilesystemLib
    PRIVATE Kmplete
)

set_target_properties(JsonLib_UnitTests PROPERTIES 
    FOLDER Kmplete/JsonLib/Tests
)

if(MSVC)
    set_target_properties(JsonLib_UnitTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    
endif()

include(Catch)
catch_discover_tests(JsonLib_UnitTests)