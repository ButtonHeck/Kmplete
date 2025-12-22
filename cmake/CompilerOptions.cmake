## ~/cmake/CompilerOptions.cmake

############################################################################
# This module contains helper SetupCompilerOptions function, that universally
# adds compiler options (c++ version, warning levels, other flags) 
# and definitions (build type, static build flag, Win32 executable flag) to a given target
############################################################################

function(SetupCompilerOptions target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
        $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor /W4 /WX /we4388 $<$<BOOL:${KMPLETE_MULTIPROCESSOR_BUILD}>:/MP>>
    )
    
    target_compile_definitions(${target}
        PRIVATE $<$<CONFIG:Debug>:KMP_CONFIG_TYPE_DEBUG>
        PRIVATE $<$<CONFIG:Release>:KMP_CONFIG_TYPE_RELEASE>
        PRIVATE $<$<CONFIG:RelWithDebInfo>:KMP_CONFIG_TYPE_RELWITHDEBINFO>
        PRIVATE $<$<CONFIG:Production>:KMP_CONFIG_TYPE_PRODUCTION>
        
        PRIVATE $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:KMP_BUILD_STATIC>
    )
    
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
        target_compile_definitions(${target} PUBLIC
            $<IF:$<BOOL:$<TARGET_PROPERTY:WIN32_EXECUTABLE>>,KMP_WINMAIN,>
        )
    endif()
endfunction()