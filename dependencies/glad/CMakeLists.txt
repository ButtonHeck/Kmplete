## ~/dependencies/glad/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(glad)
add_library(glad STATIC src/glad.c)

target_include_directories(glad
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_compile_options(glad PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:$<$<BOOL:${KMPLETE_MULTIPROCESSOR_BUILD}>:/MP>>
)
