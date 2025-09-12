## ~/src/MathLib/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


set(MathLib_UnitTests_MATH
    ${CMAKE_CURRENT_LIST_DIR}/math_tests.cpp
)

add_executable(MathLib_UnitTests
    ${MathLib_UnitTests_MATH}
)

SetupUnitTestsTargetProperties(
    MathLib_UnitTests
    Kmplete/Tests
    Catch2::Catch2WithMain MathLib
)