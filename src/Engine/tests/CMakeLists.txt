## ~/src/Engine/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

set(Kmplete_Engine_Tests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/filesystem_tests.cpp
)
source_group("Core" FILES ${Kmplete_Engine_Tests_CORE})

add_executable(Kmplete_Engine_Tests
    ${Kmplete_Engine_Tests_CORE}
)


SetupBoost(Kmplete_Engine_Tests)

target_include_directories(Kmplete_Engine_Tests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"

    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    
    PRIVATE "${GLFW_SOURCE_DIR}/include"    
    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

target_compile_options(Kmplete_Engine_Tests PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor /W4 /WX>
)

target_link_libraries(Kmplete_Engine_Tests
    PRIVATE Kmplete_Engine
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_Engine_Tests PROPERTIES 
    FOLDER Kmplete
)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(Kmplete_Engine_Tests)