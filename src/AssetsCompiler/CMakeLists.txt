## ~/src/AssetsCompiler/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(AssetsCompiler LANGUAGES CXX)

set(AssetsCompiler_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/assets_compiler.h
)
set(AssetsCompiler_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/assets_compiler.cpp
)

add_executable(AssetsCompiler
    ${AssetsCompiler_HEADERS}
    ${AssetsCompiler_SOURCES}
)

SetupBoost(AssetsCompiler)
SetupCompilerOptions(AssetsCompiler)

set_target_properties(AssetsCompiler PROPERTIES
    FOLDER Kmplete/Tools/AssetsCompiler
)

target_link_libraries(AssetsCompiler
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>

    PRIVATE BaseLib
    PRIVATE UtilsLib
    PRIVATE AssetsInterfaceLib
    PRIVATE AssetsCompilerInterfaceLib
    PRIVATE FilesystemLib
    PRIVATE JsonLib
)

if(MSVC)
    set_target_properties(AssetsCompiler PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )

endif()