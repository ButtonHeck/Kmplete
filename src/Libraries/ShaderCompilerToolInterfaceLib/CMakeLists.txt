## ~/src/Libraries/ShaderCompilerToolInterfaceLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.24)

project(ShaderCompilerToolInterfaceLib LANGUAGES CXX)

add_library(ShaderCompilerToolInterfaceLib INTERFACE)

AddTargetSourcesGroup(ShaderCompilerToolInterfaceLib "ShaderCompiler"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ShaderCompiler/shader_compiler_tool_interface.h
)

target_include_directories(ShaderCompilerToolInterfaceLib
    INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(ShaderCompilerToolInterfaceLib PROPERTIES 
    FOLDER Kmplete/Tools/ShaderCompilerToolInterfaceLib
)