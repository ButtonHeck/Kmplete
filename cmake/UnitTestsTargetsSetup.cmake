## ~/cmake/UnitTestsTargetsSetup.cmake

############################################################################
# Module with the setup function for test targets, it is responsible for
# applying compiler options, folder, linking libraries (as additional arguments),
# linking with Catch2 and (for MSVC only) setting debugger cwd to the test's executable path
############################################################################

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