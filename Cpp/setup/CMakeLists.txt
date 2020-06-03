
set(SOURCE_LIST
    ${Ecosystem_SOURCE_DIR}/setup/setup.cpp
    )

add_executable(setup ${SOURCE_LIST} ${SQLITE_SOURCES})
target_include_directories(setup PRIVATE ${Ecosystem_SOURCE_DIR}/include)
target_link_libraries(setup PRIVATE dl stdc++fs ${ECOSYSTEM_LIB})
