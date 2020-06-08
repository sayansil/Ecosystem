find_program(MAKE make)

if(NOT MAKE)
    MESSAGE( FATAL_ERROR "make executable not found" )
else()
    MESSAGE("make executable found")
    if(NOT EXISTS ${Ecosystem_SOURCE_DIR}/WebApp/extern/uWebSockets/uSockets/uSockets.a)
        execute_process(COMMAND ${MAKE} -C ${Ecosystem_SOURCE_DIR}/WebApp/extern/uWebSockets
                        COMMAND_ECHO STDOUT)
    endif()
endif()

set(SOURCE_LIST
    app.cpp
    )

add_executable(app ${SOURCE_LIST})

target_include_directories(app PRIVATE
    ${Ecosystem_SOURCE_DIR}/include
    ${Ecosystem_SOURCE_DIR}/WebApp/extern/uWebSockets/src
    ${Ecosystem_SOURCE_DIR}/WebApp/extern/uWebSockets/uSockets/src)

target_link_libraries(app PRIVATE ${ECOSYSTEM_LIB} dl stdc++fs
    ${Ecosystem_SOURCE_DIR}/WebApp/extern/uWebSockets/uSockets/uSockets.a
    z)
