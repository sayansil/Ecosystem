find_program(MAKE make)

FetchContent_Declare(
    uWebSockets
    GIT_REPOSITORY "https://github.com/uNetworking/uWebSockets"
    GIT_TAG c0ba2eb7b4f91611b202e877ea24f4476dec621c
    GIT_PROGRESS TRUE
)
FetchContent_Populate(uWebSockets)

FetchContent_Declare(
    argh
    GIT_REPOSITORY "https://github.com/adishavit/argh"
    GIT_TAG 19b37531c1305c7d74e2b93da9a001a64801b84b
    GIT_PROGRESS TRUE
)
FetchContent_Populate(argh)

if(NOT MAKE)
    MESSAGE( FATAL_ERROR "make executable not found" )
else()
    MESSAGE("make executable found")
    if(NOT EXISTS ${uWebSockets_SOURCE_DIR}/uSockets/uSockets.a)
        execute_process(COMMAND ${MAKE} -C ${uWebSockets_SOURCE_DIR}
                        )
    endif()
endif()

set(SOURCE_LIST
    app.cpp
    )

add_executable(app ${SOURCE_LIST})

target_include_directories(app PRIVATE
    ${Ecosystem_SOURCE_DIR}/include
    ${uWebSockets_SOURCE_DIR}/src
    ${uWebSockets_SOURCE_DIR}/uSockets/src
    ${argh_SOURCE_DIR})

target_link_libraries(app PRIVATE ${ECOSYSTEM_LIB} dl pthread
    ${uWebSockets_SOURCE_DIR}/uSockets/uSockets.a
    z)
