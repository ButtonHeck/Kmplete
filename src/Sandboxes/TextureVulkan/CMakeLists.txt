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


# Assets compiler configuration file generation
# --------------------------

set(path_metal 
    ${CMAKE_SOURCE_DIR}/src/Sandboxes/Resources/metal.jpg
)
set(path_marble 
    ${CMAKE_SOURCE_DIR}/src/Sandboxes/Resources/marble.jpg
)
set(path_example
    ${CMAKE_SOURCE_DIR}/src/Sandboxes/Resources/example.png
)
configure_file(TextureVulkan_assets.json.in TextureVulkan_assets.json @ONLY)


# Assets compiler invocation
# --------------------------

get_target_property(exeDir TextureVulkan RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(TextureVulkan_compile_assets
    COMMAND ${CMAKE_COMMAND} -E echo "Compiling assets for TextureVulkan done"
)
add_custom_command(TARGET TextureVulkan_compile_assets PRE_BUILD
    COMMAND AssetsCompiler 
    "--input_file_name=\"${CMAKE_CURRENT_BINARY_DIR}/TextureVulkan_assets.json\"" 
    "--output_file_name=\"${exeDir}/Data/TextureVulkan_assets.kmpdata\""
)
set_target_properties(TextureVulkan_compile_assets PROPERTIES
    FOLDER Kmplete/Sandboxes/TextureVulkan
)

add_dependencies(TextureVulkan TextureVulkan_compile_assets)