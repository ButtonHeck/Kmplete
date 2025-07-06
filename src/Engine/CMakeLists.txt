## ~/src/Engine/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Kmplete LANGUAGES CXX)

set(Kmplete_CORE
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/entry_point.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/uuid.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/program_options.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/file_dialogs.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/memory_checker.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/assertion.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/settings.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/settings_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_backend_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/system_metrics_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/timer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/rng.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/uuid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/program_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/file_dialogs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/memory_checker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/assertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_backend_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/system_metrics_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/timer.cpp
)
source_group("Core" FILES ${Kmplete_CORE})

set(Kmplete_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/window_application.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/window_application.cpp
)
source_group("Application" FILES ${Kmplete_APPLICATION})

set(Kmplete_EVENT
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/key_event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/key_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/mouse_event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/mouse_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/window_event.h
)
source_group("Event" FILES ${Kmplete_EVENT})

set(Kmplete_GRAPHICS
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/graphics_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/image.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/texture_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/texture_manager.cpp
)
source_group("Graphics" FILES ${Kmplete_GRAPHICS})

set(Kmplete_GRAPHICS_OPENGL
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_texture.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_texture.cpp
)
source_group("Graphics/OpenGL" FILES ${Kmplete_GRAPHICS_OPENGL})

set(Kmplete_LOCALIZATION
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Localization/localization_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Localization/localization_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Localization/localization_translator.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Localization/localization_library.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Localization/localization_dictionary.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_translator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_library.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Localization/localization_dictionary.cpp
)
source_group("Localization" FILES ${Kmplete_LOCALIZATION})

set(Kmplete_PROFILE
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Profile/profiler.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Profile/profiler.cpp
)
source_group("Profile" FILES ${Kmplete_PROFILE})

add_library(Kmplete
    ${Kmplete_CORE}
    ${Kmplete_APPLICATION}
    ${Kmplete_EVENT}
    ${Kmplete_GRAPHICS}
    ${Kmplete_GRAPHICS_OPENGL}
    ${Kmplete_LOCALIZATION}
    ${Kmplete_PROFILE}
)


SetupBoost(Kmplete)
SetupCompilerOptions(Kmplete)

target_include_directories(Kmplete
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${GLFW_SOURCE_DIR}/include"

    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

option(KMPLETE_USE_WINDOW_BACKEND_GLFW "Use GLFW for window management" ON)
if(${KMPLETE_USE_WINDOW_BACKEND_GLFW})
    target_compile_definitions(Kmplete PUBLIC 
        KMP_WINDOW_BACKEND_GLFW
    )
else()
    message(SEND_ERROR "No window backend is chosen")
endif()

target_compile_definitions(Kmplete
    PUBLIC KMP_DEFAULT_WINDOW_ICON_PATH=\"${KmpleteResourcesFolder}/icons/Kmplete128.png\"
    PUBLIC KMP_ICONS_FOLDER=\"${KmpleteResourcesFolder}/icons/\"
    PUBLIC KMP_FONTS_FOLDER=\"${KmpleteResourcesFolder}/fonts/\"
    PRIVATE KMP_EXPORTS
    PRIVATE KMP_ENGINE_PRIVATE
)

target_link_libraries(Kmplete 
    PUBLIC BaseLib
    PUBLIC $<IF:$<CONFIG:Production>,LoggerInterfaceLib,LoggerLib>
    PUBLIC UtilsLib
    PUBLIC VersionLib
    PUBLIC FilesystemLib
    PUBLIC JsonLib
    PUBLIC glfw
    
    PRIVATE stb_image
    PRIVATE glad
)

set_target_properties(Kmplete PROPERTIES 
    FOLDER Kmplete/Engine
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Localization
# --------------------------

set(KmpleteDomain "Engine")
target_compile_definitions(Kmplete
    PUBLIC KMP_TR_DOMAIN_ENGINE=\"${KmpleteDomain}\"
)

CreateTranslationHelperTargets(
    Kmplete Kmplete/Engine
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}/src\" \"${CMAKE_CURRENT_LIST_DIR}/include\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleteDomain}"
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleteDomain}"
)
