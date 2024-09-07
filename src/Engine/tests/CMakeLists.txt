## ~/src/Engine/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

set(Kmplete_Engine_Tests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/filesystem_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/uuid_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/json_reader_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/json_writer_tests.cpp
)
source_group("Core" FILES ${Kmplete_Engine_Tests_CORE})

set(Kmplete_Engine_Tests_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/Utils/function_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/string_utils_tests.cpp
)
source_group("Utils" FILES ${Kmplete_Engine_Tests_UTILS})

add_executable(Kmplete_Engine_Tests
    ${Kmplete_Engine_Tests_CORE}
    ${Kmplete_Engine_Tests_UTILS}
)


SetupBoost(Kmplete_Engine_Tests)
SetupCompilerOptions(Kmplete_Engine_Tests)

target_include_directories(Kmplete_Engine_Tests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"

    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    
    PRIVATE "${GLFW_SOURCE_DIR}/include"    
    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
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