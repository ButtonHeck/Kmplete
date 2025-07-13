## ~/src/FilesystemLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(FilesystemLib LANGUAGES CXX)

set(FilesystemLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Filesystem/filesystem.h
    ${CMAKE_CURRENT_LIST_DIR}/src/filesystem.cpp
)
source_group("Filesystem" FILES ${FilesystemLib_SOURCES})

add_library(FilesystemLib
    ${FilesystemLib_SOURCES}
)

SetupCompilerOptions(FilesystemLib)

target_include_directories(FilesystemLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(FilesystemLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
    PUBLIC ProfilerLib
)

target_compile_definitions(FilesystemLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(FilesystemLib PROPERTIES 
    FOLDER Kmplete/FilesystemLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()