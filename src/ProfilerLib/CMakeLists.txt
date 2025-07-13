## ~/src/ProfilerLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(ProfilerLib LANGUAGES CXX)

set(ProfilerLib_PROFILE
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Profile/profiler.h
    ${CMAKE_CURRENT_LIST_DIR}/src/profiler.cpp
)
source_group("Profile" FILES ${ProfilerLib_PROFILE})

add_library(ProfilerLib
    ${ProfilerLib_PROFILE}
)

SetupCompilerOptions(ProfilerLib)

target_include_directories(ProfilerLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(ProfilerLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
)

target_compile_definitions(ProfilerLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(ProfilerLib PROPERTIES 
    FOLDER Kmplete/ProfilerLib
)