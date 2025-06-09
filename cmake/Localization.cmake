## ~/cmake/Localization.cmake

set(Kmplete_Localization_OK TRUE)

find_package(Gettext)

find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext)
find_program(GETTEXT_MSGMERGE_EXECUTABLE msgmerge)
find_program(GETTEXT_MSGFMT_EXECUTABLE msgfmt)
find_program(GETTEXT_MSGINIT_EXECUTABLE msginit)
find_program(GETTEXT_MSGATTRIB_EXECUTABLE msgattrib)
find_program(SHELL_EXECUTABLE sh)

if(NOT Gettext_FOUND)
    set(Kmplete_Localization_OK FALSE)
    if(WIN32)
        message(WARNING "Gettext package not found! Try installing gettext or adding vendor/gettext/win path")
    elseif(UNIX)
        message(WARNING "Gettext package not found! Try installing gettext")
    endif()
endif()

if(SHELL_EXECUTABLE)
    message(STATUS "Found sh: ${SHELL_EXECUTABLE}")
else()
    message(WARNING "sh not found!")
endif()

if(GETTEXT_XGETTEXT_EXECUTABLE)
    message(STATUS "Found xgettext: ${GETTEXT_XGETTEXT_EXECUTABLE}")
else()
    set(Kmplete_Localization_OK FALSE)
    message(WARNING "xgettext not found!")
endif()

if(GETTEXT_MSGMERGE_EXECUTABLE)
    message(STATUS "Found msgmerge: ${GETTEXT_MSGMERGE_EXECUTABLE}")
else()
    set(Kmplete_Localization_OK FALSE)
    message(WARNING "msgmerge not found!")
endif()

if(GETTEXT_MSGFMT_EXECUTABLE)
    message(STATUS "Found msgfmt: ${GETTEXT_MSGFMT_EXECUTABLE}")
else()
    set(Kmplete_Localization_OK FALSE)
    message(WARNING "msgfmt not found!")
endif()

if(GETTEXT_MSGINIT_EXECUTABLE)
    message(STATUS "Found msginit: ${GETTEXT_MSGINIT_EXECUTABLE}")
else()
    set(Kmplete_Localization_OK FALSE)
    message(WARNING "msginit not found!")
endif()

if(GETTEXT_MSGATTRIB_EXECUTABLE)
    message(STATUS "Found msgattrib: ${GETTEXT_MSGATTRIB_EXECUTABLE}")
else()
    set(Kmplete_Localization_OK FALSE)
    message(WARNING "msgattrib not found!")
endif()

function(CreateTranslationHelperTargets TargetName Folder UpdateScript CompileScript Domain WorkingDirectory)
    set(UpdateTranslationsTargetName ${TargetName}_localization_update)
    message(STATUS "localization update for ${TargetName}")
    add_custom_target(${UpdateTranslationsTargetName}
        COMMAND ${CMAKE_COMMAND} -E echo "localization update for ${TargetName} done"
    )
    set_property(TARGET ${UpdateTranslationsTargetName} APPEND PROPERTY FOLDER ${Folder})
    
    add_custom_command(TARGET ${UpdateTranslationsTargetName}
        PRE_BUILD
        COMMAND ${SHELL_EXECUTABLE} ${UpdateScript} ${Domain}
        WORKING_DIRECTORY ${WorkingDirectory}
    )
    
    
    set(CompileTranslationsTargetName ${TargetName}_localization_compile)
    message(STATUS "localization compilation for ${TargetName}")
    add_custom_target(${CompileTranslationsTargetName}
        COMMAND ${CMAKE_COMMAND} -E echo "localization compilation for ${TargetName} done"
    )
    set_property(TARGET ${CompileTranslationsTargetName} APPEND PROPERTY FOLDER ${Folder})
    
    add_custom_command(TARGET ${CompileTranslationsTargetName}
        PRE_BUILD
        COMMAND ${SHELL_EXECUTABLE} ${CompileScript} ${Domain}
        WORKING_DIRECTORY ${WorkingDirectory}
    )
endfunction()