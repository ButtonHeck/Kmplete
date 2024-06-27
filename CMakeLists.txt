## ~/CMakeLists.txt

# Preamble
cmake_minimum_required(VERSION 3.20)
enable_testing()
project(Kmplete 
    VERSION 0.1.0
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

include(NoInSourceBuilds)
include(ConfigurationsSetup)
include(CompilerChecks)


# Dependencies
add_subdirectory(dependencies)


# Kmplete main targets
add_subdirectory(src)