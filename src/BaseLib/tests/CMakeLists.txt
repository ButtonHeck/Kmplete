## ~/src/BaseLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(BaseLib_UnitTests_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/platform_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/macro_tests.cpp
)

add_executable(BaseLib_UnitTests
    ${BaseLib_UnitTests_SOURCES}
)

SetupUnitTestsTargetProperties(
    BaseLib_UnitTests 
    Kmplete/BaseLib/Tests
    Catch2::Catch2WithMain BaseLib
)
