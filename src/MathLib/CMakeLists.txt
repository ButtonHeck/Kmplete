## ~/src/MathLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(MathLib LANGUAGES CXX)

set(MathLib_MATH
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Math/math.h
)
source_group("Math" FILES ${MathLib_MATH})

add_library(MathLib INTERFACE
    ${MathLib_MATH}
)

#TODO: uncomment after MathLib becomes non header-only and change INTERFACE to PUBLIC/PRIVATE
#SetupCompilerOptions(MathLib)

target_include_directories(MathLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
    INTERFACE "${glm_SOURCE_DIR}"
)

target_link_libraries(MathLib
    INTERFACE BaseLib
)

#TODO: uncomment after MathLib becomes non header-only and change INTERFACE to PUBLIC/PRIVATE
#target_compile_definitions(MathLib
#    PRIVATE KMP_EXPORTS
#)

set_target_properties(MathLib PROPERTIES
    FOLDER Kmplete/MathLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()