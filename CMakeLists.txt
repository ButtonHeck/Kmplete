## ~/CMakeLists.txt

# Preamble
cmake_minimum_required(VERSION 3.20)
enable_testing()
project(FMNCV 
    VERSION 0.1.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(NoInSourceBuilds)
include(ConfigurationsSetup)
include(CompilerChecks)

# Dependencies
add_subdirectory(dependencies)


# FMNCV main targets
add_subdirectory(src)