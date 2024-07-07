## ~/src/Editor/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Kmplete_Editor LANGUAGES CXX)

set(Kmplete_Editor_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_application.h
    ${CMAKE_CURRENT_LIST_DIR}/src/editor_application.cpp
)

add_executable(Kmplete_Editor
    ${Kmplete_Editor_SOURCES}
)

target_compile_options(Kmplete_Editor PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor>
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

file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/CopySettings$<CONFIG>.cmake"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Kmplete_Editor>/Kmplete_settings.json")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/Kmplete_settings_example.json"
        "$<TARGET_FILE_DIR:Kmplete_Editor>/Kmplete_settings.json")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Kmplete_Editor
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleteResourcesFolder} -P "CopySettings$<CONFIG>.cmake"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
