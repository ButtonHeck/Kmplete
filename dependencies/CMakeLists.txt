## ~/dependencies/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

include(FetchContent)

# Boost
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS 
	locale 
	program_options 
	REQUIRED
)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_GetProperties(GLFW)
if(NOT GLFW_POPULATED)
    FetchContent_Populate(GLFW)
    add_subdirectory(${GLFW_SOURCE_DIR} ${GLFW_BINARY_DIR})
endif()