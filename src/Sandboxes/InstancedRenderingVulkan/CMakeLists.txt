## ~/src/Sandboxes/InstancedRenderingVulkan/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(InstancedRenderingVulkan LANGUAGES CXX)

set(InstancedRenderingVulkan_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_vulkan_application.h
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_vulkan_main_frame_listener.h
)
set(InstancedRenderingVulkan_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_vulkan_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_vulkan_main_frame_listener.cpp
)

add_executable(InstancedRenderingVulkan
    ${InstancedRenderingVulkan_HEADERS}
    ${InstancedRenderingVulkan_SOURCES}
)

SetupCompilerOptions(InstancedRenderingVulkan)

set_target_properties(InstancedRenderingVulkan PROPERTIES
    FOLDER Kmplete/Sandboxes/InstancedRenderingVulkan
)

target_compile_definitions(InstancedRenderingVulkan
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(InstancedRenderingVulkan
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(InstancedRenderingVulkan PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()