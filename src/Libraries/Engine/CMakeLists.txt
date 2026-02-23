## ~/src/Libraries/Engine/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Kmplete LANGUAGES CXX)

add_library(Kmplete)

AddTargetSourcesGroup(Kmplete "Core"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/main.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/uuid.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/program_options.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/memory_checker.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/assertion.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/settings_document.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/settings_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/system_metrics_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/rng.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/uuid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/program_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/memory_checker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/assertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings_document.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/system_metrics_manager.cpp
)
AddTargetSourcesGroup(Kmplete "Application"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/window_application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/frame_listener.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/frame_listener_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/frame_listener_command.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/window_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/frame_listener.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/frame_listener_manager.cpp
)
AddTargetSourcesGroup(Kmplete "Window"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Window/window.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Window/window_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Window/window_backend_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Window/window_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Window/window_cursor.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Window/window_cursor_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_backend_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Window/window_cursor_glfw.cpp
)
AddTargetSourcesGroup(Kmplete "Event"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/key_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/mouse_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/window_events.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event_handler.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event_handler_guard.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event_dispatcher.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event_queue.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Event/event_queue.cpp
)
AddTargetSourcesGroup(Kmplete "Input"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Input/input_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Input/input_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Input/input_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Input/input_manager.cpp
)
AddTargetSourcesGroup(Kmplete "Graphics"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/graphics_base.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/graphics_surface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/physical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/logical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/swapchain.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/framebuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/command_pool.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/image.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/texture_filtering.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/texture_factory.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/font.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/graphics_surface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/physical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/swapchain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/texture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/texture_filtering.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/texture_factory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/font.cpp
)
AddTargetSourcesGroup(Kmplete "Graphics/OpenGL"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_graphics_surface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_physical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_logical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_swapchain.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_framebuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_texture.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/OpenGL/opengl_command_pool.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_graphics_surface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_physical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_logical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_swapchain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/OpenGL/opengl_texture.cpp
)
AddTargetSourcesGroup(Kmplete "Graphics/Vulkan"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_graphics_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_graphics_surface.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_physical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_context.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_logical_device.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_image_creator_delegate.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_memory_type_delegate.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_swapchain.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_framebuffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_image.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_command_pool.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_buffer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/vulkan_depth_stencil_attachment.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_graphics_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_graphics_surface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_physical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_logical_device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_image_creator_delegate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_memory_type_delegate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_swapchain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_framebuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_image.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_command_pool.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/vulkan_depth_stencil_attachment.cpp
)
AddTargetSourcesGroup(Kmplete "Graphics/Vulkan/Utils"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/Utils/initializers.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/Utils/function_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/Vulkan/Utils/result_description.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/initializers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/function_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Vulkan/Utils/result_description.cpp
)
AddTargetSourcesGroup(Kmplete "Localization"
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
AddTargetSourcesGroup(Kmplete "Assets"
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/asset.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/assets_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/font_asset.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/font_asset_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/texture_asset.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Assets/texture_asset_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/asset.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/assets_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/font_asset.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/font_asset_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/texture_asset.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Assets/texture_asset_manager.cpp
)
AddTargetSourcesGroup(Kmplete "Internal"
    ${CMAKE_CURRENT_LIST_DIR}/internal/Kmplete/Internal/default_window_icon_data.h
    ${CMAKE_CURRENT_LIST_DIR}/internal/Kmplete/Internal/error_texture_data.h
)


SetupBoost(Kmplete)
SetupCompilerOptions(Kmplete)

target_include_directories(Kmplete
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${GLFW_SOURCE_DIR}/include"
    
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/internal"
)

if(${KMPLETE_USE_WINDOW_BACKEND_GLFW})
    target_compile_definitions(Kmplete PUBLIC 
        KMP_WINDOW_BACKEND_GLFW
    )
else()
    message(SEND_ERROR "No window backend is chosen")
endif()

target_compile_definitions(Kmplete
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
    PUBLIC MathLib
    PUBLIC AssetsInterfaceLib
    PUBLIC $<IF:$<CONFIG:Production>,ProfilerInterfaceLib,ProfilerLib>
    PUBLIC TimeLib
    PUBLIC glfw
    
    PRIVATE stb_image
    PRIVATE glad
    PRIVATE freetype
    PUBLIC Vulkan::Vulkan
)

if(UNIX)
    #libfontconfig1-dev
    target_link_libraries(Kmplete PRIVATE fontconfig)
endif()

set_target_properties(Kmplete PROPERTIES 
    FOLDER Kmplete/Libraries/Engine
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
    Kmplete Kmplete/Libraries/Engine
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}/src\" \"${CMAKE_CURRENT_LIST_DIR}/include\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleteDomain}"
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleteDomain}"
)


# Installation
# --------------------------

install(TARGETS Kmplete
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)
install(DIRECTORY locale
    DESTINATION ${CMAKE_INSTALL_BINDIR} 
    COMPONENT ${KmpleteInstallEditorComponentName}
    FILES_MATCHING PATTERN "*.mo"
)
install(TARGETS glfw
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)