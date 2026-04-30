## ~/src/Sandboxes/MultiplePipelines/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(MultiplePipelines LANGUAGES CXX)

set(MultiplePipelines_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_application.h
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_frame_listener.h
)
set(MultiplePipelines_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/multiple_pipelines_frame_listener.cpp
)

add_executable(MultiplePipelines
    ${MultiplePipelines_HEADERS}
    ${MultiplePipelines_SOURCES}
)

SetupCompilerOptions(MultiplePipelines)

set_target_properties(MultiplePipelines PROPERTIES
    FOLDER Kmplete/Sandboxes/MultiplePipelines
)

target_compile_definitions(MultiplePipelines
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(MultiplePipelines
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(MultiplePipelines PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()