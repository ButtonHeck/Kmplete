## ~/src/ImGuiUtilsLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(ImGuiUtilsLib LANGUAGES CXX)

set(ImGuiUtilsLib_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/ui_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/src/ui_utils.cpp
)
source_group("Utils" FILES ${ImGuiUtilsLib_SOURCES})

aux_source_directory("${imgui_SOURCE_DIR}" IMGUI_SOURCES)
source_group("Imgui" FILES
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_library(ImGuiUtilsLib STATIC
    ${ImGuiUtilsLib_SOURCES}
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

SetupCompilerOptions(ImGuiUtilsLib)

target_include_directories(ImGuiUtilsLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC ${imgui_SOURCE_DIR}
)

target_link_libraries(ImGuiUtilsLib 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
    PUBLIC glfw
)

set_target_properties(ImGuiUtilsLib PROPERTIES 
    FOLDER Kmplete/Libraries/ImGuiUtilsLib
)