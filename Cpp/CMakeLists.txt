cmake_minimum_required(VERSION 3.5)

project(Cpp)

set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -march=native -lm -ltbb")

set(ECOSYSTEM_HEADERS
    include/nlohmann/json.hpp
    include/helper.hpp
    include/animal.hpp
    include/god.hpp
    include/database_manager.hpp
    include/EcosystemTypes.hpp
    )

set(ECOSYSTEM_SOURCES
    src/helper.cpp
    src/animal.cpp
    src/god.cpp
    src/database_manager.cpp
    )

function(setup_ecosystem)
    add_executable(setup ${CMAKE_CURRENT_SOURCE_DIR}/setup/setup.cpp)
    target_link_libraries(setup PRIVATE sqlite3)
endfunction()

function(build_ecosystem)
    find_package(ArrayFire)
    add_executable(ecosystem ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_HEADERS})
    target_include_directories(ecosystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(ecosystem ArrayFire::afcpu sqlite3)
endfunction()

function(test_arrayfire)
    find_package(ArrayFire)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ArrayFireTests)

    function(add_code name)
        add_executable(af_${name} ${SOURCE_DIR}/${name}.cpp ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_HEADERS})
        target_include_directories(af_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_link_directories(af_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_link_libraries(af_${name} ArrayFire::afcuda)
    endfunction()
    add_code(test1)
    add_code(test2)
endfunction()

function(random_test)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RandomTests)
    function(add_code name)
        add_executable(rt_${name} ${SOURCE_DIR}/${name}.cpp ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_HEADERS})
        target_include_directories(rt_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_link_directories(rt_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_link_libraries(rt_${name} PRIVATE sqlite3)
    endfunction()
#    add_code(test1)
    add_code(test2)
#    add_code(test3)
endfunction()


setup_ecosystem()
build_ecosystem()
#test_arrayfire()
random_test()
