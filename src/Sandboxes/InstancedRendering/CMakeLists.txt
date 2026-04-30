## ~/src/Sandboxes/InstancedRendering/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(InstancedRendering LANGUAGES CXX)

set(InstancedRendering_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_application.h
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_frame_listener.h
)
set(InstancedRendering_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/instanced_rendering_frame_listener.cpp
)

add_executable(InstancedRendering
    ${InstancedRendering_HEADERS}
    ${InstancedRendering_SOURCES}
)

SetupCompilerOptions(InstancedRendering)

set_target_properties(InstancedRendering PROPERTIES
    FOLDER Kmplete/Sandboxes/InstancedRendering
)

target_compile_definitions(InstancedRendering
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(InstancedRendering
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(InstancedRendering PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()