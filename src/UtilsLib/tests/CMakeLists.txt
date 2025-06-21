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

SetupUnitTestsTargetProperties(
    UtilsLib_UnitTests 
    Kmplete/UtilsLib/Tests
    Catch2::Catch2WithMain UtilsLib Kmplete
)
