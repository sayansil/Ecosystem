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

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    nlohmann
    GIT_REPOSITORY "https://github.com/nlohmann/json"
    GIT_TAG 0972f7ff0e651f09a306dba791cc42024b8642c1
    GIT_PROGRESS TRUE
)
FetchContent_Populate(nlohmann)
include_directories(${nlohmann_SOURCE_DIR}/include)

FetchContent_Declare(
    flatbuffer
    GIT_REPOSITORY "https://github.com/google/flatbuffers"
    GIT_TAG e08da4dea29449c235096a688a361fa914868b76
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(flatbuffer)
set(FLATC ${FlatBuffers_BINARY_DIR}/flatc)
message("flatc compiler at: ${FLATC}")
include_directories(${FlatBuffers_SOURCE_DIR}/include)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG 1557ab764422d5087619fea3994012066644a11e
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmtlib)

FetchContent_Declare(
    customrng
    GIT_REPOSITORY "https://github.com/Reputeless/Xoshiro-cpp"
    GIT_TAG 078bf3396c31c3fa6d6f6c785b4612fb547f7132
    GIT_PROGRESS TRUE
)
FetchContent_Populate(customrng)
#due to some reason the ${customRNG_SOURCE_DIR} variable doesn't get initialized
include_directories(${customrng_SOURCE_DIR})

FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY "https://github.com/DarkStar1997/AmalgamatedSQLite3"
    GIT_TAG 370cf3cf9ba78371c6f8b6e4d9f2bc01b88dfede
    GIT_PROGRESS TRUE
)
FetchContent_Populate(sqlite3)
include_directories(${sqlite3_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(schema)

include_directories(schema)
add_subdirectory(RandomTests)
add_subdirectory(setup)
#add_subdirectory(run)
#add_subdirectory(python)
#add_subdirectory(WebApp)

#include(CTest)
#add_subdirectory(tests)
