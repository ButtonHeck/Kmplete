## ~/cmake/TargetUtils.cmake

function(AddTargetSourcesGroup target groupName)
    set(sourcesSet ${ARGN})
    source_group(${groupName} FILES ${sourcesSet})
    target_sources(${target} PRIVATE ${sourcesSet})
endfunction()