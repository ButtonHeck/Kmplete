## ~/cmake/UnitTestsTargetsSetup.cmake

function(SetupUnitTestsTargetProperties target folder)
    SetupCompilerOptions(${target})
    
    set_target_properties(${target} PROPERTIES 
        FOLDER ${folder}
    )

    if(MSVC)
        set_target_properties(${target} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
        )
    endif()
    
    target_link_libraries(${target}
        PRIVATE ${ARGN}
    )

    include(Catch)
    catch_discover_tests(${target})
endfunction()