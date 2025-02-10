## ~/cmake/CopyTranslations.cmake

foreach(srcDir IN LISTS SRC_DIRS)
	message(STATUS "Copying \"${srcDir}\" to \"${DEST_DIR}\"")
	
	file(COPY ${srcDir} DESTINATION ${DEST_DIR}
		FILES_MATCHING PATTERN "*.mo"
	)
endforeach()
