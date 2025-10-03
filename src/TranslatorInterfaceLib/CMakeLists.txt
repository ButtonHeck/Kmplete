## ~/src/TranslatorInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(TranslatorInterfaceLib LANGUAGES CXX)

set(TranslatorInterfaceLib_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Translator/translator_interface.h
)
source_group("Translator" FILES ${TranslatorInterfaceLib_HEADERS})

add_library(TranslatorInterfaceLib INTERFACE
    ${TranslatorInterfaceLib_HEADERS}
)

target_include_directories(TranslatorInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(TranslatorInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/TranslatorInterfaceLib
)