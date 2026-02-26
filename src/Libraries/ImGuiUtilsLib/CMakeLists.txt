## ~/src/Libraries/ImGuiUtilsLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(ImGuiUtilsLib LANGUAGES CXX)

aux_source_directory("${imgui_SOURCE_DIR}" IMGUI_SOURCES)
source_group("Imgui" FILES
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_library(ImGuiUtilsLib STATIC
    ${IMGUI_SOURCES}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

AddTargetSourcesGroup(ImGuiUtilsLib "Utils"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/helper_functions.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/scope_guards.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/context.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/context_vulkan.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/implementation.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/ImGui/implementation_glfw_vulkan.h
    ${CMAKE_CURRENT_LIST_DIR}/src/helper_functions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/scope_guards.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/context_vulkan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/implementation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/implementation_glfw_vulkan.cpp
)

SetupCompilerOptions(ImGuiUtilsLib)

target_include_directories(ImGuiUtilsLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC ${imgui_SOURCE_DIR}
    PUBLIC ${imgui_SOURCE_DIR}/backends
)

if(${KMPLETE_USE_WINDOW_BACKEND_GLFW})
    target_compile_definitions(ImGuiUtilsLib PUBLIC 
        KMP_WINDOW_BACKEND_GLFW
    )
else()
    message(SEND_ERROR "No window backend is chosen")
endif()

target_link_libraries(ImGuiUtilsLib 
    PRIVATE BaseLib
    PRIVATE UtilsLib
    PRIVATE FilesystemLib
    PRIVATE $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
    PRIVATE glfw
    PRIVATE icons_fonts
    PRIVATE Vulkan::Vulkan
)

set_target_properties(ImGuiUtilsLib PROPERTIES 
    FOLDER Kmplete/Libraries/ImGuiUtilsLib
)