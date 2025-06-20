## ~/src/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

set(KmpleteResourcesFolder ${CMAKE_CURRENT_LIST_DIR}/Resources)

if(${KMPLETE_BUILD_TESTS})
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
endif()

add_subdirectory(VersionLib)
add_subdirectory(BaseLib)
add_subdirectory(LoggerLib)
add_subdirectory(UtilsLib)
add_subdirectory(FilesystemLib)
add_subdirectory(JsonLib)
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Translator)
add_subdirectory(TranslatorInterfaceLib)