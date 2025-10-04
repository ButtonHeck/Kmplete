## ~/src/AssetsInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(AssetsInterfaceLib LANGUAGES CXX)

set(AssetsInterfaceLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_interface.h
    ${CMAKE_CURRENT_LIST_DIR}/src/assets_interface.cpp
)
source_group("Assets" FILES ${AssetsInterfaceLib_SOURCES})

add_library(AssetsInterfaceLib
    ${AssetsInterfaceLib_SOURCES}
)

SetupCompilerOptions(AssetsInterfaceLib)

target_include_directories(AssetsInterfaceLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(AssetsInterfaceLib
    PUBLIC BaseLib
    PUBLIC UtilsLib
)

target_compile_definitions(AssetsInterfaceLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(AssetsInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/AssetsInterfaceLib
)