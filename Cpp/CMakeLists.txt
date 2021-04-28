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
set(ECOSYSTEM_PYTHON_MODULE "pyecosystem")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    MESSAGE("CCACHE support enabled")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

set(SQLITE_HEADERS
    ${PROJECT_SOURCE_DIR}/extern/sqlite3/include/sqlite3.h
    ${PROJECT_SOURCE_DIR}/extern/sqlite3/include/sqlite3ext.h
    )

set(SQLITE_SOURCES
    ${PROJECT_SOURCE_DIR}/extern/sqlite3/src/sqlite3.c
    )

include_directories(extern/gabime/include)

add_subdirectory(src)
add_subdirectory(RandomTests)
add_subdirectory(setup)
add_subdirectory(run)
add_subdirectory(python)
add_subdirectory(WebApp)

include(CTest)
add_subdirectory(tests)
