## ~/src/Tools/Translator/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

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

target_compile_definitions(Translator
    PRIVATE XGETTEXT_EXECUTABLE_PATH=\"${GETTEXT_XGETTEXT_EXECUTABLE}\"
    PRIVATE MSGMERGE_EXECUTABLE_PATH=\"${GETTEXT_MSGMERGE_EXECUTABLE}\"
    PRIVATE MSGFMT_EXECUTABLE_PATH=\"${GETTEXT_MSGFMT_EXECUTABLE}\"
    PRIVATE MSGINIT_EXECUTABLE_PATH=\"${GETTEXT_MSGINIT_EXECUTABLE}\"
    PRIVATE MSGATTRIB_EXECUTABLE_PATH=\"${GETTEXT_MSGATTRIB_EXECUTABLE}\"
)

target_link_libraries(Translator
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>

    PRIVATE BaseLib
    PRIVATE UtilsLib
    PRIVATE FilesystemLib
    PRIVATE TranslatorInterfaceLib
)

if(MSVC)
    set_target_properties(Translator PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    
endif()