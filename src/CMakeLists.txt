## ~/src/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

set(KmpleteResourcesFolder ${CMAKE_CURRENT_LIST_DIR}/Resources)

add_subdirectory(VersionLib)
add_subdirectory(BaseLib)
add_subdirectory(UtilsLib)
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Translator)
add_subdirectory(TranslatorInterfaceLib)