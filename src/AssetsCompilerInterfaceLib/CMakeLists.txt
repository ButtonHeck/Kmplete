## ~/src/AssetsCompilerInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(AssetsCompilerInterfaceLib LANGUAGES CXX)

set(AssetsCompilerInterfaceLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_compiler_interface.h
)
source_group("Assets" FILES ${AssetsInterfaceLib_SOURCES})

add_library(AssetsCompilerInterfaceLib INTERFACE
    ${AssetsCompilerInterfaceLib_SOURCES}
)

target_include_directories(AssetsCompilerInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(AssetsCompilerInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/AssetsCompilerInterfaceLib
)