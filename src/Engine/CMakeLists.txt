## ~/src/Engine/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Kmplete_Engine LANGUAGES CXX)


set(Kmplete_Engine_CORE
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/entry_point.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/kmplete_api.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/platform.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/types.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/uuid.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/program_options.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/filesystem.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/pointers.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/file_dialogs.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/macro.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/memory_checker.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/assertion.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/log.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/settings.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/settings_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_backend.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_backend_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/system_metrics_manager.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/nullability.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/timer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/uuid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/program_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/filesystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/file_dialogs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/memory_checker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/assertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/settings_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_backend.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_backend_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/system_metrics_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/timer.cpp
)
source_group("Core" FILES ${Kmplete_Engine_CORE})

set(Kmplete_Engine_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Application/window_application.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Application/window_application.cpp
)
source_group("Application" FILES ${Kmplete_Engine_APPLICATION})

set(Kmplete_Engine_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/function_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/string_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/string_id.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Utils/string_utils.cpp
)
source_group("Utils" FILES ${Kmplete_Engine_UTILS})

set(Kmplete_Engine_EVENT
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/key_event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/key_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/mouse_event.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/mouse_codes.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Event/window_event.h
)
source_group("Event" FILES ${Kmplete_Engine_EVENT})

set(Kmplete_Engine_JSON
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_document.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_reader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_writer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Json/json_scope.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Json/json_document.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Json/json_reader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Json/json_writer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Json/json_scope.cpp    
)
source_group("Json" FILES ${Kmplete_Engine_JSON})

set(Kmplete_Engine_GRAPHICS
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Graphics/image.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/image.cpp
)
source_group("Graphics" FILES ${Kmplete_Engine_GRAPHICS})

set(Kmplete_Engine_LOCALIZATION
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
source_group("Localization" FILES ${Kmplete_Engine_LOCALIZATION})

add_library(Kmplete_Engine
    ${Kmplete_Engine_CORE}
    ${Kmplete_Engine_APPLICATION}
    ${Kmplete_Engine_UTILS}
    ${Kmplete_Engine_EVENT}
    ${Kmplete_Engine_JSON}
    ${Kmplete_Engine_GRAPHICS}
    ${Kmplete_Engine_LOCALIZATION}
)


SetupBoost(Kmplete_Engine)
SetupCompilerOptions(Kmplete_Engine)

target_include_directories(Kmplete_Engine
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PUBLIC "${GLFW_SOURCE_DIR}/include"

    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

option(KMPLETE_USE_WINDOW_BACKEND_GLFW "Use GLFW for window management" ON)
if(${KMPLETE_USE_WINDOW_BACKEND_GLFW})
    target_compile_definitions(Kmplete_Engine PUBLIC 
        KMP_WINDOW_BACKEND_GLFW
    )
else()
    message(SEND_ERROR "No window backend is chosen")
endif()

target_compile_definitions(Kmplete_Engine
    PUBLIC $<$<CONFIG:Production>:KMP_LOG_DISABLED>
    PUBLIC $<$<CONFIG:Production>:KMP_PRODUCTION_BUILD>
    PUBLIC $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:KMP_ENGINE_STATIC>
    PUBLIC KMP_DEFAULT_WINDOW_ICON_PATH=\"${KmpleteResourcesFolder}/icons/Kmplete128.png\"
    PUBLIC KMP_FONTS_FOLDER=\"${KmpleteResourcesFolder}/fonts\"
    PRIVATE KMP_EXPORTS
    PRIVATE KMP_ENGINE_PRIVATE
    INTERFACE KMP_ENGINE_INTERFACE
)

target_link_libraries(Kmplete_Engine 
    PUBLIC Kmplete_Version
    PUBLIC glfw
    PUBLIC spdlog
    
    PRIVATE stb_image
)

set_target_properties(Kmplete_Engine PROPERTIES 
    FOLDER Kmplete/Engine
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Localization
# --------------------------

set(KmpleteEngineDomain "Engine")
target_compile_definitions(Kmplete_Engine
    PUBLIC KMP_TR_DOMAIN_ENGINE=\"${KmpleteEngineDomain}\"
)

CreateTranslationHelperTargets(Kmplete_Engine Kmplete/Engine po_update.sh po_compile.sh ${KmpleteEngineDomain} ${CMAKE_CURRENT_LIST_DIR}/scripts)