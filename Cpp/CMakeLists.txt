cmake_minimum_required(VERSION 3.5)

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    MESSAGE("CCACHE support enabled")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

project(Cpp)

set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -fopenmp -march=native")

set(ECOSYSTEM_HEADERS
    include/nlohmann/json.hpp
    include/helper.hpp
    include/stat_fetcher.hpp
    include/animal.hpp
    include/god.hpp
    include/database_manager.hpp
    include/ecosystem_types.hpp
    include/organism.hpp
    include/entity.hpp
    include/plant.hpp
    include/zmq.hpp
    include/zmq_addon.hpp
    )

set(ECOSYSTEM_SOURCES
    src/helper.cpp
    src/stat_fetcher.cpp
    src/animal.cpp
    src/god.cpp
    src/database_manager.cpp
    src/plant.cpp
    )

set(ECOSYSTEM_LIB "ecosystem")
set(ECOSYSTEM_EXE "run")

function(build_ecosystem_lib)
    add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_HEADERS} ${ECOSYSTEM_SOURCES})
    target_include_directories(${ECOSYSTEM_LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${ECOSYSTEM_LIB} PRIVATE tbb sqlite3 zmq)
endfunction()

function(setup_ecosystem)
    add_executable(setup ${CMAKE_CURRENT_SOURCE_DIR}/setup/setup.cpp)
    target_link_libraries(setup PRIVATE sqlite3)
endfunction()

function(build_ecosystem_exe)
    add_executable(${ECOSYSTEM_EXE} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    target_include_directories(${ECOSYSTEM_EXE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB})
endfunction()

build_ecosystem_lib()
setup_ecosystem()
build_ecosystem_exe()
