## ~/src/TranslatorInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(TranslatorInterfaceLib LANGUAGES CXX)

set(TranslatorApiLib_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Translator/translator_interface.h
)

add_library(TranslatorInterfaceLib INTERFACE
    ${TranslatorApiLib_HEADERS}
)

target_include_directories(TranslatorInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(TranslatorInterfaceLib PROPERTIES 
    FOLDER Kmplete/TranslatorInterfaceLib
)