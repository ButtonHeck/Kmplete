## ~/src/FilesystemLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(FilesystemLib_UnitTests_FILESYSTEM
    ${CMAKE_CURRENT_LIST_DIR}/filesystem_tests.cpp
)

add_executable(FilesystemLib_UnitTests
    ${FilesystemLib_UnitTests_FILESYSTEM}
)

SetupCompilerOptions(FilesystemLib_UnitTests)

target_link_libraries(FilesystemLib_UnitTests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE FilesystemLib
    PRIVATE Kmplete
)

set_target_properties(FilesystemLib_UnitTests PROPERTIES 
    FOLDER Kmplete/FilesystemLib/Tests
)

if(MSVC)
    set_target_properties(FilesystemLib_UnitTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    
endif()

include(Catch)
catch_discover_tests(FilesystemLib_UnitTests)