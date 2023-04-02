function(add_code name)
    add_executable(rt_${name} ${name}.cpp)
    target_include_directories(rt_${name} PRIVATE
                               ${Ecosystem_SOURCE_DIR}/include
							   )
	if(WIN32 OR APPLE)
		target_link_libraries(rt_${name} PRIVATE ${ECOSYSTEM_LIB} fmt::fmt)
	else()
		target_link_libraries(rt_${name} PRIVATE ${ECOSYSTEM_LIB} fmt::fmt dl pthread)
	endif()
	copy_dll(rt_${name} fmt)
	copy_dll(rt_${name} ${ECOSYSTEM_LIB})

endfunction()

function(add_c_code name)
    add_executable(rtc_${name} ${name}.c)
    target_include_directories(rtc_${name} PRIVATE
                               ${Ecosystem_SOURCE_DIR}/include
							   )
    target_link_libraries(rtc_${name} PRIVATE ${ECOSYSTEM_WRAPPER_LIB})
endfunction()

#######################################
# Do not comment the following tests: #
#                                     #
# - reportgen_test                    #
# - sanity_test                       #
#                                     #
#######################################

add_code(reportgen_test)
add_code(sanity_test)
add_code(test1)
add_code(test2)

add_c_code(test1)
