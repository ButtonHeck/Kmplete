## ~/src/JsonLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(JsonLib LANGUAGES CXX)

set(JsonLib_JSON
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_document.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_reader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_writer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_scope.h
    ${CMAKE_CURRENT_LIST_DIR}/src/json_document.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/json_reader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/json_writer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/json_scope.cpp
)
source_group("Json" FILES ${JsonLib_JSON})

add_library(JsonLib
    ${JsonLib_JSON}
)

SetupCompilerOptions(JsonLib)

target_include_directories(JsonLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
)

target_link_libraries(JsonLib 
    PUBLIC BaseLib
    PUBLIC LoggerLib
    PUBLIC UtilsLib
    PUBLIC FilesystemLib
)

target_compile_definitions(JsonLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(JsonLib PROPERTIES 
    FOLDER Kmplete/JsonLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()