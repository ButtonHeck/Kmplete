## ~/src/Engine/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(Kmplete_Engine LANGUAGES CXX)


set(Kmplete_Engine_CORE
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/entry_point.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/platform.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/uuid.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/program_options.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/filesystem.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/pointers.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/window_application.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/file_dialogs.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/macro.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/assertion.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/log.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/json_reader.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Core/json_writer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/uuid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/program_options.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/filesystem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/window_application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/file_dialogs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/assertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/log.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/json_reader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/json_writer.cpp
)
source_group("Core" FILES ${Kmplete_Engine_CORE})

set(Kmplete_Engine_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/function_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Utils/string_utils.h
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

add_library(Kmplete_Engine
    ${Kmplete_Engine_CORE}
    ${Kmplete_Engine_UTILS}
    ${Kmplete_Engine_EVENT}
)


set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_WARN_NEW_VERSIONS ON)

find_package(Boost REQUIRED COMPONENTS 
    locale 
    program_options 
    REQUIRED
)

target_include_directories(Kmplete_Engine
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"

    PUBLIC "${Boost_INCLUDE_DIR}"
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    
    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

target_compile_options(Kmplete_Engine PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t>
    $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor>
)

target_link_libraries(Kmplete_Engine 
    PUBLIC ${Boost_LIBRARIES}
    PUBLIC Kmplete_Version
)

set_target_properties(Kmplete_Engine PROPERTIES 
    FOLDER Kmplete
)

include(GenerateExportHeader)
generate_export_header(Kmplete_Engine 
    BASE_NAME Kmplete
    EXPORT_MACRO_NAME KMPLETE_API
)
