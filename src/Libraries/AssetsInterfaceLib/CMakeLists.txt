## ~/src/Libraries/AssetsInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(AssetsInterfaceLib LANGUAGES CXX)

add_library(AssetsInterfaceLib INTERFACE)

AddTargetSourcesGroup(AssetsInterfaceLib "Assets"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_interface.h
)

target_include_directories(AssetsInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(AssetsInterfaceLib
    INTERFACE BaseLib
)

set_target_properties(AssetsInterfaceLib PROPERTIES 
    FOLDER Kmplete/Libraries/AssetsInterfaceLib
)