## ~/dependencies/stb_image/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(stb_image)
add_library(stb_image STATIC stb_image.cpp)

target_include_directories(stb_image
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(stb_image PROPERTIES
    POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

target_compile_options(stb_image PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:$<$<BOOL:${KMPLETE_MULTIPROCESSOR_BUILD}>:/MP>>
)