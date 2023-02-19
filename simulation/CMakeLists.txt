cmake_minimum_required(VERSION 3.21)

project(Ecosystem
    DESCRIPTION "Ecosystem Simulator"
    LANGUAGES C CXX
    )


include(FetchContent)
include(cmake/download_file.cmake)
include(cmake/dll_utils.cmake)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

set(ECOSYSTEM_LIB "ecosystem")
set(ECOSYSTEM_WRAPPER_LIB ${ECOSYSTEM_LIB}_wrapper)
set(ECOSYSTEM_EXE "run")
set(ECOSYSTEM_PYTHON_MODULE "pyecosystem")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(ENABLE_CLANG_TIDY "Enable clang-tidy checks during compilation" OFF)
set(CLANG_TIDY_CHECKS "readability" CACHE STRING "Clang tidy check to be chosen by the user at configure time")
set_property(CACHE CLANG_TIDY_CHECKS PROPERTY STRINGS readability performance modernize)

if(${ENABLE_CLANG_TIDY})
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,${CLANG_TIDY_CHECKS}-*")
endif()

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    MESSAGE("CCACHE support enabled")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE 1)

set(FETCHCONTENT_QUIET FALSE)
get_filename_component(fc_cache "../.fetchcontent_cache"
                       REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
set(FETCHCONTENT_BASE_DIR ${fc_cache})

FetchContent_Declare(
    nlohmann
    GIT_REPOSITORY "https://github.com/nlohmann/json"
    GIT_TAG v3.11.2
    GIT_PROGRESS TRUE
)
FetchContent_Populate(nlohmann)
include_directories(${nlohmann_SOURCE_DIR}/include)

set(FLATBUFFERS_BUILD_TESTS OFF CACHE INTERNAL "")
FetchContent_Declare(
    flatbuffer
    GIT_REPOSITORY "https://github.com/google/flatbuffers"
    GIT_TAG v22.9.29
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(flatbuffer)
set(FLATC ${FlatBuffers_BINARY_DIR}/flatc)
message("flatc compiler at: ${FLATC}")
include_directories(${FlatBuffers_SOURCE_DIR}/include)

FetchContent_Declare(
    fmtlib
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG 9.1.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(fmtlib)
include_directories(${fmtlib_SOURCE_DIR}/include)

FetchContent_Declare(
    customrng
    GIT_REPOSITORY "https://github.com/Reputeless/Xoshiro-cpp"
    GIT_TAG 19bcbb2ce0ed158233187f524fd0964c105a65b3
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

FetchContent_Declare(
    whereami
    GIT_REPOSITORY "https://github.com/gpakosz/whereami"
    GIT_TAG ba364cd54fd431c76c045393b6522b4bff547f50
    GIT_PROGRESS TRUE
)
FetchContent_Populate(whereami)
include_directories(${whereami_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(schema)

include_directories(schema/cpp)
add_subdirectory(RandomTests)
# add_subdirectory(setup)
add_subdirectory(run)
#add_subdirectory(python)
#add_subdirectory(WebApp)

include(CTest)
add_subdirectory(tests)
