## ~/src/Libraries/ShaderCompilerLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(ShaderCompilerLib LANGUAGES CXX)

add_library(ShaderCompilerLib)

AddTargetSourcesGroup(ShaderCompilerLib "ShaderCompiler"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ShaderCompiler/compile.h
    ${CMAKE_CURRENT_LIST_DIR}/src/compile.cpp
)

SetupCompilerOptions(ShaderCompilerLib)

target_include_directories(ShaderCompilerLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(ShaderCompilerLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
    PUBLIC $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
    
    PRIVATE FilesystemLib
    PRIVATE Vulkan::shaderc_combined
)

target_compile_definitions(ShaderCompilerLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(ShaderCompilerLib PROPERTIES 
    FOLDER Kmplete/Libraries/ShaderCompilerLib
)


# Installation
# --------------------------

install(TARGETS ShaderCompilerLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)