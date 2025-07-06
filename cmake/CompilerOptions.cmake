## ~/cmake/CompilerOptions.cmake

function(SetupCompilerOptions target)
    target_compile_options(${target} PUBLIC
        $<$<CXX_COMPILER_ID:GNU>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
        $<$<CXX_COMPILER_ID:Clang>:-std=c++20 -fno-char8_t -Wall -Wextra -Werror>
        $<$<CXX_COMPILER_ID:MSVC>:-std:c++20 /Zc:char8_t- /Zc:preprocessor /W4 /WX>
    )
    
    target_compile_definitions(${target}
        PUBLIC $<$<CONFIG:Debug>:KMP_CONFIG_TYPE_DEBUG>
        PUBLIC $<$<CONFIG:Release>:KMP_CONFIG_TYPE_RELEASE>
        PUBLIC $<$<CONFIG:RelWithDebInfo>:KMP_CONFIG_TYPE_RELWITHDEBINFO>
        PUBLIC $<$<CONFIG:Production>:KMP_CONFIG_TYPE_PRODUCTION>
        
        PUBLIC $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:KMP_BUILD_STATIC>
    )
    
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
        target_compile_definitions(${target} PUBLIC
            $<IF:$<BOOL:$<TARGET_PROPERTY:WIN32_EXECUTABLE>>,KMP_WINMAIN,>
        )
    endif()
endfunction()