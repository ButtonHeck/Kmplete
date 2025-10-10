## ~/src/AssetsInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(AssetsInterfaceLib LANGUAGES CXX)

set(AssetsInterfaceLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_interface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_compiler_interface.h
)
source_group("Assets" FILES ${AssetsInterfaceLib_SOURCES})

add_library(AssetsInterfaceLib INTERFACE
    ${AssetsInterfaceLib_SOURCES}
)

target_include_directories(AssetsInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(AssetsInterfaceLib
    INTERFACE BaseLib
    INTERFACE UtilsLib
)

set_target_properties(AssetsInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/AssetsInterfaceLib
)