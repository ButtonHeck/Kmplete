## ~/src/Sandboxes/PushConstants/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(PushConstants LANGUAGES CXX)

set(PushConstants_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/push_constants_application.h
    ${CMAKE_CURRENT_LIST_DIR}/push_constants_frame_listener.h
)
set(PushConstants_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/push_constants_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/push_constants_frame_listener.cpp
)

add_executable(PushConstants
    ${PushConstants_HEADERS}
    ${PushConstants_SOURCES}
)

SetupCompilerOptions(PushConstants)

set_target_properties(PushConstants PROPERTIES
    FOLDER Kmplete/Sandboxes/PushConstants
)

target_compile_definitions(PushConstants
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(PushConstants
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(PushConstants PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()