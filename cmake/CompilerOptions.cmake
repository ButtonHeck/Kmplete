## ~/cmake/CompilerOptions.cmake

function(SetupCompilerOptions target)
    target_compile_options(${target} PUBLIC
        $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
        $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor /W4 /WX>
    )
endfunction()