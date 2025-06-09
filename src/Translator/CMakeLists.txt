## ~/src/Translator/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Translator LANGUAGES CXX)

add_executable(Translator main.cpp)

SetupBoost(Translator)
SetupCompilerOptions(Translator)

set_target_properties(Translator PROPERTIES 
    FOLDER Kmplete/Translator
)

target_compile_definitions(Translator
    PRIVATE XGETTEXT_EXECUTABLE_PATH=\"${GETTEXT_XGETTEXT_EXECUTABLE}\"
    PRIVATE MSGMERGE_EXECUTABLE_PATH=\"${GETTEXT_MSGMERGE_EXECUTABLE}\"
    PRIVATE MSGFMT_EXECUTABLE_PATH=\"${GETTEXT_MSGFMT_EXECUTABLE}\"
    PRIVATE MSGINIT_EXECUTABLE_PATH=\"${GETTEXT_MSGINIT_EXECUTABLE}\"
    PRIVATE MSGATTRIB_EXECUTABLE_PATH=\"${GETTEXT_MSGATTRIB_EXECUTABLE}\"
)