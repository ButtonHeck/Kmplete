## ~/src/ProfilerLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(ProfilerLib LANGUAGES CXX)


# Profiler library for anything but "Production" build

add_library(ProfilerLib)

AddTargetSourcesGroup(ProfilerLib "Profile"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Profile/profiler.h
    ${CMAKE_CURRENT_LIST_DIR}/src/profiler.cpp
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
    FOLDER Kmplete/Libraries/ProfilerLib
)


# Profiler library for "Production" build

add_library(ProfilerInterfaceLib INTERFACE)

AddTargetSourcesGroup(ProfilerInterfaceLib "Profile"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Profile/profiler.h
)

target_compile_definitions(ProfilerInterfaceLib
    INTERFACE KMP_CONFIG_TYPE_PRODUCTION
)

target_include_directories(ProfilerInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(ProfilerInterfaceLib PROPERTIES 
    FOLDER Kmplete/Libraries/ProfilerLib
)


# Installation
# --------------------------

install(TARGETS ProfilerLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)