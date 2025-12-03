## ~/src/AssetsCompilerInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(AssetsCompilerInterfaceLib LANGUAGES CXX)

add_library(AssetsCompilerInterfaceLib INTERFACE)

AddTargetSourcesGroup(AssetsCompilerInterfaceLib "Assets"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_compiler_interface.h
)

target_include_directories(AssetsCompilerInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(AssetsCompilerInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/AssetsCompilerInterfaceLib
)