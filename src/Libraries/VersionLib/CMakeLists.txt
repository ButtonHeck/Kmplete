## ~/src/Libraries/VersionLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

find_package(Git REQUIRED)

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE Kmplete_GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(result)
    message(FATAL_ERROR "Failed to get git hash: ${result}")
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE Kmplete_GIT_HASH_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(result)
    message(FATAL_ERROR "Failed to get short git hash: ${result}")
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE result
    OUTPUT_VARIABLE Kmplete_GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(result)
    message(FATAL_ERROR "Failed to get git branch: ${result}")
endif()

configure_file(kmplete_version.cpp.in kmplete_version.cpp @ONLY)

add_library(VersionLib STATIC 
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Version/kmplete_version.h
    ${CMAKE_CURRENT_BINARY_DIR}/kmplete_version.cpp
)

set_target_properties(VersionLib PROPERTIES 
    FOLDER Kmplete/Libraries/VersionLib
    POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

target_include_directories(VersionLib 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)