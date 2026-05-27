## ~/src/Sandboxes/UniformBuffers/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(UniformBuffers LANGUAGES CXX)

set(UniformBuffers_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/uniform_buffers_application.h
    ${CMAKE_CURRENT_LIST_DIR}/uniform_buffers_frame_listener.h
)
set(UniformBuffers_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/uniform_buffers_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/uniform_buffers_frame_listener.cpp
)
set(UniformBuffers_SHADERS
    ${KmpleteSandboxResourcesFolder}/uniform_buffers.vert
    ${KmpleteSandboxResourcesFolder}/uniform_buffers.frag
)
source_group("Shaders" FILES ${UniformBuffers_SHADERS})

add_executable(UniformBuffers
    ${UniformBuffers_HEADERS}
    ${UniformBuffers_SOURCES}
    ${UniformBuffers_SHADERS}
)

SetupCompilerOptions(UniformBuffers)

set_target_properties(UniformBuffers PROPERTIES
    FOLDER Kmplete/Sandboxes/UniformBuffers
)

target_compile_definitions(UniformBuffers
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(UniformBuffers
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(UniformBuffers PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()