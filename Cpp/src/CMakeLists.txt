
#set(ECOSYSTEM_HEADERS
#    ${Ecosystem_SOURCE_DIR}/include/nlohmann/json.hpp
#    ${Ecosystem_SOURCE_DIR}/include/helper.hpp
#    ${Ecosystem_SOURCE_DIR}/include/stat_fetcher.hpp
#    ${Ecosystem_SOURCE_DIR}/include/animal.hpp
#    ${Ecosystem_SOURCE_DIR}/include/god.hpp
#    ${Ecosystem_SOURCE_DIR}/include/database_manager.hpp
#    ${Ecosystem_SOURCE_DIR}/include/ecosystem_types.hpp
#    ${Ecosystem_SOURCE_DIR}/include/organism.hpp
#    ${Ecosystem_SOURCE_DIR}/include/entity.hpp
#    ${Ecosystem_SOURCE_DIR}/include/plant.hpp
#    ${Ecosystem_SOURCE_DIR}/include/zmq.hpp
#    ${Ecosystem_SOURCE_DIR}/include/zmq_addon.hpp
#    )

set(ECOSYSTEM_SOURCES
    helper.cpp
    stat_fetcher.cpp
    animal.cpp
    god.cpp
    database_manager.cpp
    plant.cpp
    )

add_library(${ECOSYSTEM_LIB} SHARED ${ECOSYSTEM_SOURCES} ${SQLITE_SOURCES})
target_include_directories(${ECOSYSTEM_LIB} PRIVATE ${Ecosystem_SOURCE_DIR}/include)
target_link_libraries(${ECOSYSTEM_LIB} PRIVATE stdc++fs)
