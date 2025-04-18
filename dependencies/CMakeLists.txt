## ~/dependencies/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

## Dependencies without FetchContent
# glad
message("---- Kmplete dependencies: glad")
add_subdirectory(glad)
set_target_properties(glad PROPERTIES FOLDER Dependencies/glad)

# stb_image
message("---- Kmplete dependencies: stb_image")
add_subdirectory(stb_image)
set_target_properties(stb_image PROPERTIES FOLDER Dependencies/stb_image)

# icons_fonts
message("---- Kmplete dependencies: icons_fonts")
add_subdirectory(icons_fonts)


## Dependencies fetched via FetchContent
## --------------------------------------------------------
include(FetchContent)

# GLFW
message("---- Kmplete dependencies: GLFW")
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_GetProperties(glfw)

if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    
    option(GLFW_BUILD_DOCS OFF)
    option(GLFW_BUILD_TESTS OFF)
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_INSTALL OFF)
    
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
    
    set_target_properties(glfw PROPERTIES FOLDER Dependencies/GLFW)
    set_target_properties(update_mappings PROPERTIES FOLDER Dependencies/GLFW)
endif()


# Dear ImGui
message("---- Kmplete dependencies: ImGui")
FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.7-docking
)
FetchContent_GetProperties(imgui)

if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    
    # ImGui doesn't contain CMakeLists, thus manually set the variable
    set(imgui_SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/imgui-src" PARENT_SCOPE)
endif()


# PFD
message("---- Kmplete dependencies: pfd")
FetchContent_Declare(pfd
    GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
    GIT_TAG 7f852d88a480020d7f91957cbcefe514fc95000c
)
FetchContent_GetProperties(pfd)

if(NOT pfd_POPULATED)
    FetchContent_Populate(pfd)
    
    add_subdirectory(${pfd_SOURCE_DIR} ${pfd_BINARY_DIR})
endif()


# RapidJSON
message("---- Kmplete dependencies: RapidJSON")
FetchContent_Declare(rapidjson
    GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
    GIT_TAG ab1842a2dae061284c0a62dca1cc6d5e7e37e346
)
FetchContent_GetProperties(rapidjson)

if(NOT rapidjson_POPULATED)
    FetchContent_Populate(rapidjson)
    
    set(RAPIDJSON_BUILD_CXX11 OFF CACHE BOOL "" FORCE)
    set(RAPIDJSON_BUILD_CXX17 ON CACHE BOOL "" FORCE)
    set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    
    add_subdirectory(${rapidjson_SOURCE_DIR} ${rapidjson_BINARY_DIR})
    
    set_target_properties(travis_doc PROPERTIES FOLDER Dependencies/RapidJSON)
endif()


# spdlog
message("---- Kmplete dependencies: spdlog")
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_GetProperties(spdlog)

if(NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
    
    set_target_properties(spdlog PROPERTIES FOLDER Dependencies/spdlog)
endif()


# Catch2
message("---- Kmplete dependencies: Catch2")
FetchContent_Declare(catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
)
FetchContent_GetProperties(catch2)

if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
    
    set_target_properties(Catch2 PROPERTIES FOLDER Dependencies/Catch2)
    set_target_properties(Catch2WithMain PROPERTIES FOLDER Dependencies/Catch2)
endif()


# glm
message("---- Kmplete dependencies: glm")
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    
    set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)
    
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
    
    set_target_properties(glm PROPERTIES FOLDER Dependencies/glm)
endif()


# entt
message("---- Kmplete dependencies: EnTT")
FetchContent_Declare(entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.13.2
)
FetchContent_GetProperties(entt)

if(NOT entt_POPULATED)
    FetchContent_Populate(entt)
    
    add_subdirectory(${entt_SOURCE_DIR} ${entt_BINARY_DIR})    
endif()


# FreeType
message("---- Kmplete dependencies: FreeType")
FetchContent_Declare(freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG VER-2-13-2
)
FetchContent_GetProperties(freetype)

if(NOT freetype_POPULATED)
    FetchContent_Populate(freetype)
    
    set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
    set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
    
    add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
    
    set_target_properties(freetype PROPERTIES FOLDER Dependencies/FreeType)
endif()


# openal-soft
message("---- Kmplete dependencies: OpenAL-soft")
FetchContent_Declare(openal-soft
    GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG 1.23.1 
)
FetchContent_GetProperties(openal-soft)

if(NOT openal-soft_POPULATED)
    FetchContent_Populate(openal-soft)
    
    set(ALSOFT_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)
    if(WIN32)
        set(ALSOFT_DLOPEN OFF CACHE BOOL "" FORCE)
    endif()
    
    add_subdirectory(${openal-soft_SOURCE_DIR} ${openal-soft_BINARY_DIR})
    
    set_target_properties(alrecord PROPERTIES FOLDER Dependencies/OpenAL/Examples)
    set_target_properties(altonegen PROPERTIES FOLDER Dependencies/OpenAL/Examples)
    set_target_properties(common PROPERTIES FOLDER Dependencies/OpenAL)
    set_target_properties(ex-common PROPERTIES FOLDER Dependencies/OpenAL)
    set_target_properties(OpenAL PROPERTIES FOLDER Dependencies/OpenAL)
    set_target_properties(openal-info PROPERTIES FOLDER Dependencies/OpenAL)
endif()
