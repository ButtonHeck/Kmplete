## ~/src/LoggerLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(LoggerLib LANGUAGES CXX)


# Logger library for anything but "Production" build

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
    PUBLIC "${spdlog_SOURCE_DIR}/include"
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


# Logger library for "Production" build

set(LoggerInterfaceLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Log/log.h
)
source_group("Log" FILES ${LoggerInterfaceLib_SOURCES})

add_library(LoggerInterfaceLib INTERFACE
    ${LoggerInterfaceLib_SOURCES}
)

target_compile_definitions(LoggerInterfaceLib
    INTERFACE KMP_CONFIG_TYPE_PRODUCTION
)

target_include_directories(LoggerInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
    INTERFACE "${spdlog_SOURCE_DIR}/include"
)

set_target_properties(LoggerInterfaceLib PROPERTIES 
    FOLDER Kmplete/LoggerLib
)