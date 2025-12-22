## ~/cmake/NoInSourceBuilds.cmake

############################################################################
# Module for protecting cmake builds in the same directory as the source
############################################################################

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()