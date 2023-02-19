set(ECOSYSTEM_SOURCES
    # helper.cpp
    stat_fetcher.cpp
    god.cpp
    organism.cpp
    helper.cpp
    database_manager.cpp
    setup.cpp
    species_constants.cpp
    # logger.cpp
    ${sqlite3_SOURCE_DIR}/src/sqlite3.c
    ${whereami_SOURCE_DIR}/src/whereami.c
    )

set(ECOSYSTEM_WRAPPER_SOURCES
    ecosystem_wrapper.cpp
    )

add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_SOURCES})
add_library(${ECOSYSTEM_WRAPPER_LIB} SHARED ${ECOSYSTEM_WRAPPER_SOURCES})
target_link_libraries(${ECOSYSTEM_WRAPPER_LIB} PRIVATE ${ECOSYSTEM_LIB})

target_include_directories(${ECOSYSTEM_LIB} PRIVATE
                           ${Ecosystem_SOURCE_DIR}/include
                           ${Ecosystem_SOURCE_DIR}/schema/cpp)

target_include_directories(${ECOSYSTEM_WRAPPER_LIB} PRIVATE
                           ${Ecosystem_SOURCE_DIR}/include
                           ${Ecosystem_SOURCE_DIR}/schema/cpp)

if(WIN32 OR APPLE)
    target_link_libraries(${ECOSYSTEM_LIB} PRIVATE fmt::fmt)
else()
    target_link_libraries(${ECOSYSTEM_LIB} PRIVATE fmt::fmt dl pthread)
endif()

copy_dll(${ECOSYSTEM_WRAPPER_LIB} fmt)

if(${ENABLE_CLANG_TIDY})
    set_target_properties(${ECOSYSTEM_LIB} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()
