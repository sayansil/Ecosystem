cmake_minimum_required(VERSION 3.5)

project(Ecosystem
    DESCRIPTION "Ecosystem Simulator"
    LANGUAGES C CXX
    )


include(FetchContent)
include(cmake/download_file.cmake)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

set(ECOSYSTEM_LIB "ecosystem")
set(ECOSYSTEM_EXE "run")

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    MESSAGE("CCACHE support enabled")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_FLAGS "-std=c++2a -O3 -fopenmp -march=native -ltbb")

set(SQLITE_HEADERS
    ${PROJECT_SOURCE_DIR}/include/sqlite3.h
    ${PROJECT_SOURCE_DIR}/include/sqlite3ext.h
    )

set(SQLITE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/sqlite3.c
    )

add_subdirectory(src)
add_subdirectory(RandomTests)
add_subdirectory(setup)
add_subdirectory(run)

include(CTest)
add_subdirectory(tests)
