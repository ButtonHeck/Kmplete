## ~/src/Editor/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Kmplete_Editor LANGUAGES CXX)

set(Kmplete_Editor_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_application.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl_opengl_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_ui_impl_opengl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ui_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ui_utils.cpp
)
source_group("Editor" FILES ${Kmplete_Editor_SOURCES})

if(WIN32)
	set(KMPLETE_WIN_ICO_PATH "${KmpleteResourcesFolder}/icons/Kmplete64.ico")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/win/kmplete_editor.rc_in" "${CMAKE_CURRENT_BINARY_DIR}/kmplete_editor.rc")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/win/resource.h" "${CMAKE_CURRENT_BINARY_DIR}/resource.h" COPYONLY)
	set(Kmplete_Editor_WINRC
		"${CMAKE_CURRENT_BINARY_DIR}/resource.h"
		"${CMAKE_CURRENT_BINARY_DIR}/kmplete_editor.rc"
	)
	source_group("Resources" FILES ${Kmplete_Editor_WINRC})
endif()

aux_source_directory("${imgui_SOURCE_DIR}" IMGUI_SOURCES)
source_group("Imgui" FILES
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_executable(Kmplete_Editor
    ${Kmplete_Editor_SOURCES}
    ${Kmplete_Editor_WINRC}
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)


SetupCompilerOptions(Kmplete_Editor)

target_include_directories(Kmplete_Editor
    PRIVATE ${imgui_SOURCE_DIR}
)

target_link_libraries(Kmplete_Editor
    PRIVATE Kmplete_Engine
)

if(MSVC)
    target_link_options(Kmplete_Editor 
        PRIVATE $<$<CONFIG:RELWITHDEBINFO>:/PROFILE>
    )
    
    set_property(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} PROPERTY 
        VS_STARTUP_PROJECT Kmplete_Editor
    )
    
    set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY 
        VS_STARTUP_PROJECT Kmplete_Editor
    )
    
    set_target_properties(Kmplete_Editor PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    
endif()

set_target_properties(Kmplete_Editor PROPERTIES 
    FOLDER Kmplete
)

set(KmpleteCopySettingsScriptName "CopySettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${KmpleteCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Kmplete_Editor>/Kmplete_settings.json")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmplete_settings_example.json"
        "$<TARGET_FILE_DIR:Kmplete_Editor>/Kmplete_settings.json")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Kmplete_Editor
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleteResourcesFolder} -P ${KmpleteCopySettingsScriptName}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
