set(ECOSYSTEM_SOURCES
    helper.cpp
    stat_fetcher.cpp
    animal.cpp
    god.cpp
    database_manager.cpp
    plant.cpp
    schema.cpp
    StopWatch.cpp
    species_constants.cpp
    logger.cpp
    ${sqlite3_SOURCE_DIR}/src/sqlite3.c
    )

add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_SOURCES})
target_include_directories(${ECOSYSTEM_LIB} PRIVATE 
                           ${Ecosystem_SOURCE_DIR}/include)
target_link_libraries(${ECOSYSTEM_LIB} PRIVATE tbb)
