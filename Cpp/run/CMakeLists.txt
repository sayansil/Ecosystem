set(SOURCE_LIST
    ${Ecosystem_SOURCE_DIR}/run/main.cpp
    )

add_executable(${ECOSYSTEM_EXE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_EXE} PRIVATE ${Ecosystem_SOURCE_DIR}/include)
target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} dl stdc++fs)
