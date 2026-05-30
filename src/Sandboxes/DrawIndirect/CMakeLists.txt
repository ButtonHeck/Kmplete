## ~/src/Sandboxes/DrawIndirect/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(DrawIndirect LANGUAGES CXX)

set(DrawIndirect_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/draw_indirect_application.h
    ${CMAKE_CURRENT_LIST_DIR}/draw_indirect_frame_listener.h
)
set(DrawIndirect_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/draw_indirect_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/draw_indirect_frame_listener.cpp
)
set(DrawIndirect_SHADERS
    ${KmpleteSandboxResourcesFolder}/draw_indirect.vert
    ${KmpleteSandboxResourcesFolder}/draw_indirect.frag
)
source_group("Shaders" FILES ${DrawIndirect_SHADERS})

add_executable(DrawIndirect
    ${DrawIndirect_HEADERS}
    ${DrawIndirect_SOURCES}
    ${DrawIndirect_SHADERS}
)

SetupCompilerOptions(DrawIndirect)

set_target_properties(DrawIndirect PROPERTIES
    FOLDER Kmplete/Sandboxes/DrawIndirect
)

target_compile_definitions(DrawIndirect
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(DrawIndirect
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(DrawIndirect PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()