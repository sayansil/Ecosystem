function(add_code name)
    add_executable(rt_${name} ${name}.cpp ${SQLITE_SOURCES})
    target_include_directories(rt_${name} PRIVATE ${Ecosystem_SOURCE_DIR}/include ${Ecosystem_SOURCE_DIR}/extern/nlohmann/include)
    target_link_libraries(rt_${name} PRIVATE ${ECOSYSTEM_LIB} dl tbb)
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
#    add_code(unit_tests)
add_code(test1)
add_code(test2)
#    add_code(test3)
#    add_code(client)
#    add_code(server)
