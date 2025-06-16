## ~/src/LoggerLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(LoggerLib LANGUAGES CXX)

set(LoggerLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Log/log.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Log/log_api.h
    ${CMAKE_CURRENT_LIST_DIR}/src/log.cpp
)
source_group("Log" FILES ${LoggerLib_SOURCES})

add_library(LoggerLib
    ${LoggerLib_SOURCES}
)

SetupCompilerOptions(LoggerLib)

target_include_directories(LoggerLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(LoggerLib 
    PUBLIC BaseLib
    PUBLIC spdlog
)

target_compile_definitions(LoggerLib
    PUBLIC $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:KMP_BUILD_STATIC>
    PRIVATE KMP_LOG_EXPORTS
)

set_target_properties(LoggerLib PROPERTIES 
    FOLDER Kmplete/LoggerLib
)