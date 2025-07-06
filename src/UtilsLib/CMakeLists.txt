## ~/src/UtilsLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(UtilsLib LANGUAGES CXX)

set(UtilsLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/function_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/string_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/string_id.h
    ${CMAKE_CURRENT_LIST_DIR}/src/string_utils.cpp
)
source_group("Utils" FILES ${UtilsLib_SOURCES})

add_library(UtilsLib
    ${UtilsLib_SOURCES}
)

SetupCompilerOptions(UtilsLib)

target_include_directories(UtilsLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(UtilsLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
)

target_compile_definitions(UtilsLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(UtilsLib PROPERTIES 
    FOLDER Kmplete/UtilsLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()