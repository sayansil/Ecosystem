function(copy_dll TARGET_NAME DLL_NAME)
	add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:${DLL_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}>)
endfunction()
