## ~/dependencies/icons_fonts/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

add_library(icons_fonts INTERFACE
    forkawesome-webfont.h
)

target_include_directories(icons_fonts
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(icons_fonts PROPERTIES FOLDER Dependencies/icons_fonts)