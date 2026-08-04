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
    PRIVATE harfbuzz
    PRIVATE freetype
)

if(MSVC)
    set_target_properties(TextRendering PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()


# Localization
# --------------------------

set(TextRenderingDomain "TextRendering")
target_compile_definitions(TextRendering
    PRIVATE KMP_TR_DOMAIN_TEXT_RENDERING=\"${TextRenderingDomain}\"
)

get_target_property(exeDir TextRendering RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(TextRendering_copy_translations
    COMMAND ${CMAKE_COMMAND} -E echo "Copying translations for TextRendering done"
)
add_custom_command(TARGET TextRendering_copy_translations PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale" 
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
    VERBATIM
)
set_target_properties(TextRendering_copy_translations PROPERTIES
    FOLDER Kmplete/Sandboxes/TextRendering
)

add_dependencies(TextRendering TextRendering_copy_translations)

CreateTranslationHelperTargets(
    TextRendering Kmplete/Sandboxes/TextRendering
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${TextRenderingDomain} --gettext_binaries_paths=\"${CMAKE_BINARY_DIR}/src/Tools/Translator/translator_gettext_paths.json\""
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${TextRenderingDomain} --gettext_binaries_paths=\"${CMAKE_BINARY_DIR}/src/Tools/Translator/translator_gettext_paths.json\""
)