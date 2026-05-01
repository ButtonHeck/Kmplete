## ~/src/Libraries/UtilsLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(UtilsLib LANGUAGES CXX)

add_library(UtilsLib)

AddTargetSourcesGroup(UtilsLib "Utils"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/function_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/string_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/vector_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/memory_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/src/string_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/memory_utils.cpp
)

SetupCompilerOptions(UtilsLib)

target_include_directories(UtilsLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(UtilsLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
    PUBLIC $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
)

target_compile_definitions(UtilsLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(UtilsLib PROPERTIES 
    FOLDER Kmplete/Libraries/UtilsLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Installation
# --------------------------

install(TARGETS UtilsLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)