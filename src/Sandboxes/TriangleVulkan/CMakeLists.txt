## ~/src/Sandboxes/TriangleVulkan/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(TriangleVulkan LANGUAGES CXX)

set(TriangleVulkan_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/triangle_vulkan_application.h
    ${CMAKE_CURRENT_LIST_DIR}/triangle_vulkan_main_frame_listener.h
)
set(TriangleVulkan_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/triangle_vulkan_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/triangle_vulkan_main_frame_listener.cpp
)

add_executable(TriangleVulkan
    ${TriangleVulkan_HEADERS}
    ${TriangleVulkan_SOURCES}
)

SetupCompilerOptions(TriangleVulkan)

set_target_properties(TriangleVulkan PROPERTIES
    FOLDER Kmplete/Sandboxes/TriangleVulkan
)

target_compile_definitions(TriangleVulkan
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(TriangleVulkan
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(TriangleVulkan PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()