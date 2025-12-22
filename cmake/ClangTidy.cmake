## ~/cmake/ClangTidy.cmake

############################################################################
# Experimental module for connecting clang-tidy analyzer to a given target.
# If clang-tidy program is not found, then AddClangTidy function is noop
############################################################################

find_program(CLANG-TIDY_PATH clang-tidy OPTIONAL)
if(NOT CLANG-TIDY_PATH)
    message(WARNING "clang-tidy is not found")
endif()


function(AddClangTidy target)
    if(NOT CLANG-TIDY_PATH)
        message(WARNING "cannot apply clang-tidy to ${target}")
        return()
    endif()

    set_target_properties(${target} PROPERTIES
        CXX_CLANG_TIDY "${CLANG-TIDY_PATH};-checks=*;--warnings-as-errors=*"
    )
endfunction()