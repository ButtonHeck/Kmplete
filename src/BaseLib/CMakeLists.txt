## ~/src/BaseLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(BaseLib LANGUAGES CXX)

set(BaseLib_BASE
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/kmplete_api.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/platform.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/macro.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/nullability.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/pointers.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/types_aliases.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Base/optional.h
)
source_group("Base" FILES ${BaseLib_BASE})

add_library(BaseLib INTERFACE
    ${BaseLib_BASE}
)

target_include_directories(BaseLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(BaseLib PROPERTIES 
    FOLDER Kmplete/BaseLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()