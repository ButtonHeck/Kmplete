## ~/src/FileDialogsLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(FileDialogsLib LANGUAGES CXX)

set(FileDialogsLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/FileDialogs/file_dialogs.h
    ${CMAKE_CURRENT_LIST_DIR}/src/file_dialogs.cpp
)
source_group("File Dialogs" FILES ${FileDialogsLib_SOURCES})

add_library(FileDialogsLib
    ${FileDialogsLib_SOURCES}
)

SetupCompilerOptions(FileDialogsLib)

target_include_directories(FileDialogsLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"

    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

target_link_libraries(FileDialogsLib
    PUBLIC BaseLib

    PRIVATE FilesystemLib
    PRIVATE UtilsLib
)

target_compile_definitions(FileDialogsLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(FileDialogsLib PROPERTIES 
    FOLDER Kmplete/FileDialogsLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()