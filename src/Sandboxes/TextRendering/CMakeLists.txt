## ~/src/Sandboxes/TextRendering/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(TextRendering LANGUAGES CXX)

set(TextRendering_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/text_rendering_application.h
    ${CMAKE_CURRENT_LIST_DIR}/text_rendering_frame_listener.h
)
set(TextRendering_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/text_rendering_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/text_rendering_frame_listener.cpp
)
set(TextRendering_SHADERS
    ${KmpleteSandboxResourcesFolder}/text_rendering.vert
    ${KmpleteSandboxResourcesFolder}/text_rendering.frag
)
source_group("Shaders" FILES ${TextRendering_SHADERS})

add_executable(TextRendering
    ${TextRendering_HEADERS}
    ${TextRendering_SOURCES}
    ${TextRendering_SHADERS}
)

SetupCompilerOptions(TextRendering)

set_target_properties(TextRendering PROPERTIES
    FOLDER Kmplete/Sandboxes/TextRendering
)

target_compile_definitions(TextRendering
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(TextRendering
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(TextRendering PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()