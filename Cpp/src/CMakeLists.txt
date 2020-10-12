set(ECOSYSTEM_SOURCES
    helper.cpp
    stat_fetcher.cpp
    animal.cpp
    god.cpp
    database_manager.cpp
    plant.cpp
    schema.cpp
    )

add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_SOURCES} ${SQLITE_SOURCES})
target_include_directories(${ECOSYSTEM_LIB} PRIVATE ${Ecosystem_SOURCE_DIR}/include ${Ecosystem_SOURCE_DIR}/extern/nlohmann/include)
target_link_libraries(${ECOSYSTEM_LIB} PRIVATE tbb)
