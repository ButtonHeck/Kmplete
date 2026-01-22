## ~/src/Libraries/TranslatorInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(TranslatorInterfaceLib LANGUAGES CXX)

add_library(TranslatorInterfaceLib INTERFACE)

AddTargetSourcesGroup(TranslatorInterfaceLib "Translator"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Translator/translator_interface.h
)

target_include_directories(TranslatorInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(TranslatorInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/TranslatorInterfaceLib
)