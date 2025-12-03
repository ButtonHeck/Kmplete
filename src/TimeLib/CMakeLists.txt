## ~/src/TimeLib/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

project(TimeLib LANGUAGES CXX)

set(TimeLib_TIME
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Time/clock.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Time/timer.h
    ${CMAKE_CURRENT_LIST_DIR}/include/Kmplete/Time/helper_functions.h
    ${CMAKE_CURRENT_LIST_DIR}/src/clock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/timer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/helper_functions.cpp
)
source_group("Time" FILES ${TimeLib_TIME})

add_library(TimeLib
    ${TimeLib_TIME}
)

SetupCompilerOptions(TimeLib)

target_include_directories(TimeLib
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(TimeLib
    PUBLIC BaseLib
)

target_compile_definitions(TimeLib
    PRIVATE KMP_EXPORTS
)

set_target_properties(TimeLib PROPERTIES 
    FOLDER Kmplete/Libraries/TimeLib
)

if(${KMPLETE_BUILD_TESTS})
    add_subdirectory(tests)
endif()


# Installation
# --------------------------

install(TARGETS TimeLib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} 
        COMPONENT ${KmpleteInstallEditorComponentName}
)