find_program(MAKE make)

if(NOT MAKE)
    MESSAGE( FATAL_ERROR "make executable not found" )
else()
    MESSAGE("make executable found")
    if(NOT EXISTS ${Ecosystem_SOURCE_DIR}/extern/uWebSockets/uSockets/uSockets.a)
        execute_process(COMMAND ${MAKE} -C ${Ecosystem_SOURCE_DIR}/extern/uWebSockets
                        )
    endif()
endif()

set(SOURCE_LIST
    app.cpp
    )

add_executable(app ${SOURCE_LIST})

target_include_directories(app PRIVATE
    ${Ecosystem_SOURCE_DIR}/include
    ${Ecosystem_SOURCE_DIR}/extern/uWebSockets/src
    ${Ecosystem_SOURCE_DIR}/extern/uWebSockets/uSockets/src
    ${Ecosystem_SOURCE_DIR}/extern/nlohmann/include
    ${Ecosystem_SOURCE_DIR}/extern/argh)

target_link_libraries(app PRIVATE ${ECOSYSTEM_LIB} dl stdc++fs pthread
    ${Ecosystem_SOURCE_DIR}/extern/uWebSockets/uSockets/uSockets.a
    z)
