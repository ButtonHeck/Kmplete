## ~/src/Sandboxes/UniformDynamic/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(UniformDynamic LANGUAGES CXX)

set(UniformDynamic_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/uniform_dynamic_application.h
    ${CMAKE_CURRENT_LIST_DIR}/uniform_dynamic_frame_listener.h
)
set(UniformDynamic_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/uniform_dynamic_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/uniform_dynamic_frame_listener.cpp
)

add_executable(UniformDynamic
    ${UniformDynamic_HEADERS}
    ${UniformDynamic_SOURCES}
)

SetupCompilerOptions(UniformDynamic)

set_target_properties(UniformDynamic PROPERTIES
    FOLDER Kmplete/Sandboxes/UniformDynamic
)

target_compile_definitions(UniformDynamic
    PRIVATE KMP_SANDBOX_RESOURCES_FOLDER=\"${KmpleteSandboxResourcesFolder}/\"
)

target_link_libraries(UniformDynamic
    PRIVATE Kmplete
    PRIVATE ImGuiUtilsLib
    PRIVATE Vulkan::Vulkan
)

if(MSVC)
    set_target_properties(UniformDynamic PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()