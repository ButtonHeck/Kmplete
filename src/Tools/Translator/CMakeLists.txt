## ~/src/Tools/Translator/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(Translator LANGUAGES CXX)

set(Translator_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/translator.h
)
set(Translator_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/translator.cpp
)

add_executable(Translator
    ${Translator_HEADERS}
    ${Translator_SOURCES}
)

SetupBoost(Translator)
SetupCompilerOptions(Translator)

set_target_properties(Translator PROPERTIES 
    FOLDER Kmplete/Tools/Translator
)

target_link_libraries(Translator
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>

    PRIVATE BaseLib
    PRIVATE UtilsLib
    PRIVATE JsonLib
    PRIVATE FilesystemLib
    PRIVATE TranslatorInterfaceLib
)

if(MSVC)
    set_target_properties(Translator PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    
endif()


# Translator gettext paths generation
# --------------------------
set(XGETTEXT_PATH ${GETTEXT_XGETTEXT_EXECUTABLE})
set(MSGMERGE_PATH ${GETTEXT_MSGMERGE_EXECUTABLE})
set(MSGFMT_PATH ${GETTEXT_MSGFMT_EXECUTABLE})
set(MSGINIT_PATH ${GETTEXT_MSGINIT_EXECUTABLE})
set(MSGATTRIB_PATH ${GETTEXT_MSGATTRIB_EXECUTABLE})

configure_file(translator_gettext_paths.json.in translator_gettext_paths.json @ONLY)

get_target_property(exeDir Translator RUNTIME_OUTPUT_DIRECTORY)

add_custom_command(TARGET Translator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_BINARY_DIR}/translator_gettext_paths.json"
    "${exeDir}"
)


# Installation
# --------------------------

install(TARGETS Translator
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)