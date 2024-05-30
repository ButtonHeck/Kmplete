## ~/src/Version/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    RESULT_VARIABLE result
    OUTPUT_VARIABLE FMNCV_GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(result)
    message(FATAL_ERROR "Failed to get git hash: ${result}")
endif()

configure_file(fmncv_version.cpp.in fmncv_version.cpp @ONLY)
add_library(FMNCV_Version STATIC ${CMAKE_CURRENT_BINARY_DIR}/fmncv_version.cpp)
set_target_properties(FMNCV_Version PROPERTIES FOLDER FMNCV)