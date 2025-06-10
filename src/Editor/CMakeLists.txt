## ~/src/Editor/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Editor LANGUAGES CXX)

set(Editor_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_application.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_compositor.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_compositor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl_opengl_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl_opengl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ui_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui_identifiers.h
    ${CMAKE_CURRENT_LIST_DIR}/src/shortcuts.h
    ${CMAKE_CURRENT_LIST_DIR}/src/localization_base.h
)
source_group("Editor" FILES ${Editor_SOURCES})

if(WIN32)
    set(KMPLETE_WIN_ICO_PATH "${KmpleteResourcesFolder}/icons/Kmplete64.ico")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/win/kmplete_editor.rc_in" "${CMAKE_CURRENT_BINARY_DIR}/kmplete_editor.rc")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/win/resource.h" "${CMAKE_CURRENT_BINARY_DIR}/resource.h" COPYONLY)
    set(Editor_WINRC
        "${CMAKE_CURRENT_BINARY_DIR}/resource.h"
        "${CMAKE_CURRENT_BINARY_DIR}/kmplete_editor.rc"
    )
    source_group("Resources" FILES ${Editor_WINRC})
endif()

aux_source_directory("${imgui_SOURCE_DIR}" IMGUI_SOURCES)
source_group("Imgui" FILES
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_executable(Editor
    ${Editor_SOURCES}
    ${Editor_WINRC}
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)


SetupCompilerOptions(Editor)

target_include_directories(Editor
    PRIVATE ${imgui_SOURCE_DIR}
)

target_link_libraries(Editor
    PRIVATE Kmplete
    PRIVATE icons_fonts
)

if(MSVC)
    target_link_options(Editor 
        PRIVATE $<$<CONFIG:RELWITHDEBINFO>:/PROFILE>
    )
    
    set_property(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} PROPERTY 
        VS_STARTUP_PROJECT Editor
    )
    
    set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY 
        VS_STARTUP_PROJECT Editor
    )
    
    set_target_properties(Editor PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    
endif()

set_target_properties(Editor PROPERTIES 
    FOLDER Kmplete/Editor
)

set(EditorCopySettingsScriptName "CopySettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${EditorCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Editor>/Kmplete_settings.json")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmplete_settings_example.json"
        "$<TARGET_FILE_DIR:Editor>/Kmplete_settings.json")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Editor
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleteResourcesFolder} -P ${EditorCopySettingsScriptName}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)


# Localization
# --------------------------

set(EditorDomain "Editor")
target_compile_definitions(Editor
    PRIVATE KMP_TR_DOMAIN_EDITOR=\"${EditorDomain}\"
)

get_target_property(exeDir Editor RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Editor_copy_translations
    COMMAND ${CMAKE_COMMAND} -E echo "Copying translations for Editor done"
)
add_custom_command(TARGET Editor_copy_translations PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale;${CMAKE_CURRENT_LIST_DIR}/../Engine/locale" 
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
)
set_target_properties(Editor_copy_translations PROPERTIES
    FOLDER Kmplete/Editor
)

add_dependencies(Editor Editor_copy_translations)

CreateTranslationHelperTargets(
    Editor Kmplete/Editor
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}/src\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${EditorDomain}"
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${EditorDomain}"
)
