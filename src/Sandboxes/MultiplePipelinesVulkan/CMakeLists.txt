## ~/src/Sandboxes/MultiplePipelinesVulkan/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(MultiplePipelinesVulkan LANGUAGES CXX)

set(MultiplePipelinesVulkan_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_vulkan_application.h
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_vulkan_main_frame_listener.h
)
set(MultiplePipelinesVulkan_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_vulkan_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_vulkan_main_frame_listener.cpp
)

add_executable(MultiplePipelinesVulkan
    ${MultiplePipelinesVulkan_HEADERS}
    ${MultiplePipelinesVulkan_SOURCES}
)

SetupCompilerOptions(MultiplePipelinesVulkan)

set_target_properties(MultiplePipelinesVulkan PROPERTIES
    FOLDER Kmplete/Sandboxes/MultiplePipelinesVulkan
)

target_compile_definitions(MultiplePipelinesVulkan
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(MultiplePipelinesVulkan
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(MultiplePipelinesVulkan PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()