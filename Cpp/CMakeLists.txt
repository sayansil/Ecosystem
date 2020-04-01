cmake_minimum_required(VERSION 3.5)

project(Cpp)

set(CMAKE_CXX_FLAGS "-std=c++17 -O3 -fopenmp -march=native -lm")

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

function(setup_ecosystem)
    add_executable(setup ${CMAKE_CURRENT_SOURCE_DIR}/setup/setup.cpp)
    target_link_libraries(setup PRIVATE sqlite3)
endfunction()

function(build_ecosystem)
    add_executable(ecosystem ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_HEADERS})
    target_include_directories(ecosystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(ecosystem sqlite3 zmq)
endfunction()

function(random_test)
    set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RandomTests)
    function(add_code name)
        add_executable(rt_${name} ${SOURCE_DIR}/${name}.cpp ${ECOSYSTEM_SOURCES} ${ECOSYSTEM_HEADERS})
        target_include_directories(rt_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_link_directories(rt_${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
        target_link_libraries(rt_${name} PRIVATE sqlite3 zmq pthread)
    endfunction()
    add_code(generateReport)
    add_code(test1)
    add_code(test2)
    add_code(test3)
#    add_code(client)
#    add_code(server)
endfunction()


setup_ecosystem()
build_ecosystem()
random_test()
