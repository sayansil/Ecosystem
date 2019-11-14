cmake_minimum_required(VERSION 3.5)

project(Cpp)

set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -march=native")

function(build_ecosystem)
    set(HEADERS
        include/nlohmann/json.hpp
        include/helper.hpp
        include/organism.hpp
        )

    set(SOURCES
        src/main.cpp
        src/helper.cpp
        src/organism.cpp
        )
    add_executable(ecosystem ${SOURCES} ${HEADERS})
    target_include_directories(ecosystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endfunction()

function(test_arrayfire)
    find_package(ArrayFire)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ArrayFireTests)

    function(add_code name)
        add_executable(af_${name} ${SOURCE_DIR}/${name}.cpp)
        target_link_libraries(af_${name} ArrayFire::afcuda)
    endfunction()
    add_code(test1)
endfunction()

function(random_test)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RandomTests)
    function(add_code name)
        add_executable(rt_${name} ${SOURCE_DIR}/${name}.cpp)
        target_include_directories(rt_${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    endfunction()
    add_code(test1)
    add_code(test2)
endfunction()


#build_ecosystem()
#test_arrayfire()
random_test()
