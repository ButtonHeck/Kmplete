## ~/src/Tools/ShaderCompiler/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(ShaderCompilerTool LANGUAGES CXX)

set(ShaderCompilerTool_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/shader_compiler_tool.h
)
set(ShaderCompilerTool_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shader_compiler_tool.cpp
)

add_executable(ShaderCompilerTool
    ${ShaderCompilerTool_HEADERS}
    ${ShaderCompilerTool_SOURCES}
)

SetupBoost(ShaderCompilerTool)
SetupCompilerOptions(ShaderCompilerTool)

set_target_properties(ShaderCompilerTool PROPERTIES
    FOLDER Kmplete/Tools/ShaderCompilerTool
)

target_link_libraries(ShaderCompilerTool
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>

    PRIVATE BaseLib
    PRIVATE UtilsLib
    PRIVATE ShaderCompilerLib
    PRIVATE ShaderCompilerToolInterfaceLib
    PRIVATE FilesystemLib
    PRIVATE JsonLib
)

if(MSVC)
    set_target_properties(ShaderCompilerTool PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )

endif()


# Installation
# --------------------------

install(TARGETS ShaderCompilerTool
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)