## ~/src/MathLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(MathLib LANGUAGES CXX)

set(MathLib_MATH
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Math/math.h
    ${CMAKE_CURRENT_LIST_DIR}/src/math.cpp
)
source_group("Math" FILES ${MathLib_MATH})

add_library(MathLib
    ${MathLib_MATH}
)

SetupCompilerOptions(MathLib)

target_include_directories(MathLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC ${glm_SOURCE_DIR}
)

target_link_libraries(MathLib
    PUBLIC BaseLib
)

target_compile_definitions(MathLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(MathLib PROPERTIES
    FOLDER Kmplete/MathLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()