## ~/dependencies/icons_fonts/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

add_library(icons_fonts INTERFACE)

target_include_directories(icons_fonts
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)