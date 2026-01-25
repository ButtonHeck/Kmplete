## ~/src/Sandboxes/TriangleOpenGL/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(TriangleOpenGL LANGUAGES CXX)

set(TriangleOpenGL_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/triangle_opengl_application.h
    ${CMAKE_CURRENT_LIST_DIR}/triangle_opengl_main_frame_listener.h
)
set(TriangleOpenGL_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/triangle_opengl_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/triangle_opengl_main_frame_listener.cpp
)

add_executable(TriangleOpenGL
    ${TriangleOpenGL_HEADERS}
    ${TriangleOpenGL_SOURCES}
)

SetupCompilerOptions(TriangleOpenGL)

set_target_properties(TriangleOpenGL PROPERTIES
    FOLDER Kmplete/Sandboxes/TriangleOpenGL
)

target_link_libraries(TriangleOpenGL
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE glad
)

if(MSVC)
    set_target_properties(TriangleOpenGL PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()