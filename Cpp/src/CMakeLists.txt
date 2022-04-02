set(ECOSYSTEM_SOURCES
    # helper.cpp
    stat_fetcher.cpp
    god.cpp
    organism.cpp
    helper.cpp
    database_manager.cpp
    schema.cpp
    # StopWatch.cpp
    species_constants.cpp
    # logger.cpp
    ${sqlite3_SOURCE_DIR}/src/sqlite3.c
    )

add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_SOURCES})
target_include_directories(${ECOSYSTEM_LIB} PRIVATE
                           ${Ecosystem_SOURCE_DIR}/include
                           ${Ecosystem_SOURCE_DIR}/schema)
if(WIN32 OR APPLE)
    target_link_libraries(${ECOSYSTEM_LIB} PRIVATE fmt::fmt)
else()
    target_link_libraries(${ECOSYSTEM_LIB} PRIVATE fmt::fmt dl pthread)
endif()
