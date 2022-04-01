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

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_LIB}_EXPORTS.h)
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${ECOSYSTEM_LIB}
		BASE_NAME ${ECOSYSTEM_LIB}
		EXPORT_MACRO_NAME ${ECOSYSTEM_LIB}_EXPORTS
		EXPORT_FILE_NAME ${ECOSYSTEM_LIB}_EXPORTS.h
		STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(${ECOSYSTEM_LIB} PRIVATE
                           ${Ecosystem_SOURCE_DIR}/include
                           ${Ecosystem_SOURCE_DIR}/schema)
target_link_libraries(${ECOSYSTEM_LIB} PRIVATE fmt::fmt)
