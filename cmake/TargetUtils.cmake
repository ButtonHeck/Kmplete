## ~/cmake/TargetUtils.cmake

############################################################################
# Additional utility functions for target stuff
#
# AddTargetSourcesGroup example usage:
# add_library(myLib)
# ...
# AddTargetSourcesGroup(
#   myLib "SourceGroupName"
#   src1.cpp
#   ...
#   srcN.cpp
# )
############################################################################

function(AddTargetSourcesGroup target groupName)
    set(sourcesSet ${ARGN})
    source_group(${groupName} FILES ${sourcesSet})
    target_sources(${target} PRIVATE ${sourcesSet})
endfunction()