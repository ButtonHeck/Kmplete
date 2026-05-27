## ~/src/Sandboxes/StorageBuffers/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(StorageBuffers LANGUAGES CXX)

set(StorageBuffers_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/storage_buffers_application.h
    ${CMAKE_CURRENT_LIST_DIR}/storage_buffers_frame_listener.h
)
set(StorageBuffers_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/storage_buffers_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/storage_buffers_frame_listener.cpp
)
set(StorageBuffers_SHADERS
    ${KmpleteSandboxResourcesFolder}/storage_buffers.vert
    ${KmpleteSandboxResourcesFolder}/storage_buffers.frag
)
source_group("Shaders" FILES ${StorageBuffers_SHADERS})

add_executable(StorageBuffers
    ${StorageBuffers_HEADERS}
    ${StorageBuffers_SOURCES}
    ${StorageBuffers_SHADERS}
)

SetupCompilerOptions(StorageBuffers)

set_target_properties(StorageBuffers PROPERTIES
    FOLDER Kmplete/Sandboxes/StorageBuffers
)

target_compile_definitions(StorageBuffers
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(StorageBuffers
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(StorageBuffers PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()