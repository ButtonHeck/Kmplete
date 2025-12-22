## ~/cmake/CopyTranslations.cmake

############################################################################
# This module contains the command for copying .mo translation files and supposed to 
# be used as a part of a custom_command. That custom command is responsible for passing
# both SRC_DIRS and DEST_DIR arguments
#
# example usage:
# add_custom_command(
#   TARGET someTarget COMMAND ${CMAKE_COMMAND}
#   -D DEST_DIR=... -D SRC_DIRS=...
#   -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
#   VERBATIM
# )
############################################################################

foreach(srcDir IN LISTS SRC_DIRS)
	message(STATUS "Copying \"${srcDir}\" to \"${DEST_DIR}\"")
	
	file(COPY ${srcDir} DESTINATION ${DEST_DIR}
		FILES_MATCHING PATTERN "*.mo"
	)
endforeach()
