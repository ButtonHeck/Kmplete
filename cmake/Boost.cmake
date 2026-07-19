## ~/cmake/Boost.cmake

############################################################################
# CMake module for including Boost components e.g.:
# "SetupBoost(someTarget)" - include and link boost libraries to someTarget.
# Boost v1.83.0 is used during development
############################################################################

# Force cmake pre 3.30 policy for handling Boost finding
cmake_policy(SET CMP0167 OLD)

set(Boost_NO_SYSTEM_PATHS ${KMPLETE_IGNORE_SYSTEM_BOOST})
set(Boost_USE_MULTITHREADED ON)
if (BUILD_SHARED_LIBS)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_SHARED_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_SHARED_LIBS OFF)
endif()
set(Boost_NO_WARN_NEW_VERSIONS ON)

find_package(Boost REQUIRED COMPONENTS 
    locale 
    program_options 
    filesystem
)

function(SetupBoost target)
    target_include_directories(${target}
        PUBLIC "${Boost_INCLUDE_DIR}"
    )
    target_link_libraries(${target}
        PUBLIC ${Boost_LIBRARIES}
    )
endfunction()
