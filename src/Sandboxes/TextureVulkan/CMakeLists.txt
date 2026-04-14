## ~/src/Sandboxes/TextureVulkan/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(TextureVulkan LANGUAGES CXX)

set(TextureVulkan_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/texture_vulkan_application.h
    ${CMAKE_CURRENT_LIST_DIR}/texture_vulkan_main_frame_listener.h
)
set(TextureVulkan_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/texture_vulkan_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/texture_vulkan_main_frame_listener.cpp
)

add_executable(TextureVulkan
    ${TextureVulkan_HEADERS}
    ${TextureVulkan_SOURCES}
)

SetupCompilerOptions(TextureVulkan)

set_target_properties(TextureVulkan PROPERTIES
    FOLDER Kmplete/Sandboxes/TextureVulkan
)

target_compile_definitions(TextureVulkan
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(TextureVulkan
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(TextureVulkan PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()