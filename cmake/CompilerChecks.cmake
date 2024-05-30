## ~/cmake/CompilerChecks.cmake

include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    check_cxx_compiler_flag(-std=c++20 CXX_20_OK)
    check_cxx_compiler_flag(-fno-char8_t CXX_NO_CHAR8_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK)
        message(SEND_ERROR "GNU compiler failed to check necessary flags support!")
    else()
        message(STATUS "GNU compiler check necessary flags OK")
    endif()
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    check_cxx_compiler_flag(-std=c++20 CXX_20_OK)
    check_cxx_compiler_flag(-fno-char8_t CXX_NO_CHAR8_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK)
        message(SEND_ERROR "Clang compiler failed to check necessary compiler flags support!")
    else()
        message(STATUS "Clang compiler check necessary flags OK")
    endif()
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    check_cxx_compiler_flag(-std:c++20 CXX_20_OK)
    check_cxx_compiler_flag(/Zc:char8_t- CXX_NO_CHAR8_OK)
    check_cxx_compiler_flag(/Zc:preprocessor CXX_PREPROCESSOR_OK)
    if(NOT CXX_20_OK OR NOT CXX_NO_CHAR8_OK OR NOT CXX_PREPROCESSOR_OK)
        message(SEND_ERROR "MSVC compiler failed to check necessary compiler flags support!")
    else()
        message(STATUS "MSVC compiler check necessary flags OK")
    endif()
    
else()
    message(WARNING "Cannot check compiler flags for: ${CMAKE_CXX_COMPILER_ID}")

endif()