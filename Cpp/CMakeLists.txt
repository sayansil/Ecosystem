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
    spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog"
    GIT_TAG af0d805be43365fa03e92b5fe143e1b50e443cf7
    GIT_PROGRESS TRUE
)
FetchContent_Populate(spdlog)
include_directories(${spdlog_SOURCE_DIR}/include)

FetchContent_Declare(
    customRNG
    GIT_REPOSITORY "https://github.com/Reputeless/Xoshiro-cpp"
    GIT_TAG 078bf3396c31c3fa6d6f6c785b4612fb547f7132
    GIT_PROGRESS TRUE
)
FetchContent_Populate(customRNG)
#due to some reason the ${customRNG_SOURCE_DIR} variable doesn't get initialized
include_directories(${customRNG_SOURCE_DIR})

FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY "https://github.com/DarkStar1997/AmalgamatedSQLite3"
    GIT_TAG 370cf3cf9ba78371c6f8b6e4d9f2bc01b88dfede
    GIT_PROGRESS TRUE
)
FetchContent_Populate(sqlite3)
include_directories(${sqlite3_SOURCE_DIR}/include)

FetchContent_Declare(
    refl-cpp
    GIT_REPOSITORY "https://github.com/veselink1/refl-cpp"
    GIT_TAG 4684ff412bda23db3c757307f4fa16b54a5c5788
    GIT_PROGRESS TRUE
)
FetchContent_Populate(refl-cpp)
include_directories(${refl-cpp_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(RandomTests)
add_subdirectory(setup)
add_subdirectory(run)
#add_subdirectory(python)
#add_subdirectory(WebApp)

include(CTest)
add_subdirectory(tests)
