cmake_minimum_required(VERSION 3.5)

project(Cpp)

set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -march=native -lm")

set(ECOSYSTEM_HEADERS
    include/nlohmann/json.hpp
    include/helper.hpp
    include/organism.hpp
    include/herbivore.hpp
    )

set(ECOSYSTEM_SOURCES
    src/helper.cpp
    src/organism.cpp
    src/herbivore.cpp
    )

function(build_ecosystem)
    find_package(ArrayFire)
    add_executable(ecosystem ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_HEADERS})
    target_include_directories(ecosystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(ecosystem ArrayFire::afcpu)
endfunction()

function(test_arrayfire)
    find_package(ArrayFire)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ArrayFireTests)

    function(add_code name)
        add_executable(af_${name} ${SOURCE_DIR}/${name}.cpp)
        target_link_libraries(af_${name} ArrayFire::afcpu)
    endfunction()
    add_code(test1)
endfunction()

function(random_test)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RandomTests)
    function(add_code name)
        add_executable(rt_${name} ${SOURCE_DIR}/${name}.cpp)
        target_include_directories(rt_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_link_directories(rt_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    endfunction()
#    add_code(test1)
#    add_code(test2)
    add_code(test3)
endfunction()


build_ecosystem()
#test_arrayfire()
random_test()
