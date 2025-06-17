## ~/src/UtilsLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(UtilsLib_UnitTests_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/function_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/string_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/string_id_tests.cpp
)

add_executable(UtilsLib_UnitTests
    ${UtilsLib_UnitTests_UTILS}
)

SetupCompilerOptions(UtilsLib_UnitTests)

target_link_libraries(UtilsLib_UnitTests
    PRIVATE Catch2::Catch2WithMain
    PRIVATE UtilsLib
    PRIVATE Kmplete
)

set_target_properties(UtilsLib_UnitTests PROPERTIES 
    FOLDER Kmplete/UtilsLib/Tests
)