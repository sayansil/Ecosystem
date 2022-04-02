set(SOURCE_LIST
    setup.cpp
    )

function(copy_dll TARGET_NAME DLL_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:${DLL_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}>)
endfunction()

add_executable(setup ${SOURCE_LIST})
target_include_directories(setup PRIVATE ${Ecosystem_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/src)

if(WIN32 OR APPLE)
    target_link_libraries(setup PRIVATE ${ECOSYSTEM_LIB})
else()
    target_link_libraries(setup PRIVATE ${ECOSYSTEM_LIB} dl pthread)
endif()

copy_dll(setup ${ECOSYSTEM_LIB})
copy_dll(setup fmt)
