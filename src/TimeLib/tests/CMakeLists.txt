## ~/src/TimeLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(TimeLib_UnitTests_TIME
    ${CMAKE_CURRENT_LIST_DIR}/timer_tests.cpp
)

add_executable(TimeLib_UnitTests
    ${TimeLib_UnitTests_TIME}
)

SetupUnitTestsTargetProperties(
    TimeLib_UnitTests 
    Kmplete/Tests
    Catch2::Catch2WithMain BaseLib TimeLib
)