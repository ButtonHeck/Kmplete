## ~/src/Sandboxes/Triangle/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(Triangle LANGUAGES CXX)

set(Triangle_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/triangle_application.h
    ${CMAKE_CURRENT_LIST_DIR}/triangle_frame_listener.h
    ${CMAKE_CURRENT_LIST_DIR}/events.h
)
set(Triangle_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/triangle_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/triangle_frame_listener.cpp
)

add_executable(Triangle
    ${Triangle_HEADERS}
    ${Triangle_SOURCES}
)

SetupCompilerOptions(Triangle)

set_target_properties(Triangle PROPERTIES
    FOLDER Kmplete/Sandboxes/Triangle
)

target_compile_definitions(Triangle
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(Triangle
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(Triangle PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()