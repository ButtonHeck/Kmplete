## ~/cmake/Boost.cmake

set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_WARN_NEW_VERSIONS ON)

find_package(Boost REQUIRED COMPONENTS 
    locale 
    program_options 
    REQUIRED
)

function(SetupBoost target)
    target_include_directories(${target}
        PUBLIC "${Boost_INCLUDE_DIR}"
    )
    target_link_libraries(${target}
        PUBLIC ${Boost_LIBRARIES}
    )
endfunction()