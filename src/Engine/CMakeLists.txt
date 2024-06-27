## ~/src/Engine/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

add_library(Kmplete_Engine)

add_subdirectory(src)
add_subdirectory(include)

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
    PRIVATE "${pfd_SOURCE_DIR}"
)

target_link_libraries(Kmplete_Engine 
    PUBLIC ${Boost_LIBRARIES}
)

set_target_properties(Kmplete_Engine PROPERTIES 
    FOLDER Kmplete
)

include(GenerateExportHeader)
generate_export_header(Kmplete_Engine 
    BASE_NAME Kmplete
    EXPORT_MACRO_NAME KMPLETE_API
)
