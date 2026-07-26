## ~/cmake/Packaging.cmake

############################################################################
# Module for setting packaging properties of the project used via cpack command
############################################################################

function(SetupPackaging)
    set(CPACK_PACKAGE_NAME "KmpleteEditor")
    set(CPACK_PACKAGE_VENDOR "Buttonheck")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Kmplete engine editor")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_COMPONENTS_ALL KmpleteInstallEditor)
    set(CPACK_COMPONENT_INSTALL ON)

    if(UNIX)
        set(CPACK_GENERATOR "DEB")
        set(CPACK_PACKAGE_CONTACT "not yet")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "not yet")
        set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/Kmplete")
    else()
        set(CPACK_GENERATOR "NSIS")
    endif()

    include(CPack)
endfunction()