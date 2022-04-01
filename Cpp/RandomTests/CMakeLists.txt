function(copy_dll TARGET_NAME DLL_NAME)
	if(WIN32)
		add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:${DLL_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}>)
	endif()
endfunction()

function(add_code name)
    add_executable(rt_${name} ${name}.cpp)
    target_include_directories(rt_${name} PRIVATE
                               ${Ecosystem_SOURCE_DIR}/include
							   ${CMAKE_BINARY_DIR}/src)
    target_link_libraries(rt_${name} PRIVATE ${ECOSYSTEM_LIB} fmt::fmt)
	copy_dll(rt_${name} fmt)
	copy_dll(rt_${name} ${ECOSYSTEM_LIB})
	
endfunction()


#######################################
# Do not comment the following tests: #
#                                     #
# - reportgen_test                    #
# - sanity_test                       #
#                                     #
#######################################

# add_code(reportgen_test)
# add_code(sanity_test)
#    add_code(unit_tests)
# add_code(test1)
# add_code(test2)
add_code(test3)
#    add_code(client)
#    add_code(server)
