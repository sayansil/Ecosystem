set(SOURCE_LIST
    main.cpp
    )

add_executable(${ECOSYSTEM_EXE} ${SOURCE_LIST})
target_include_directories(${ECOSYSTEM_EXE} PRIVATE ${Ecosystem_SOURCE_DIR}/include ${Ecosystem_SOURCE_DIR}/extern/nlohmann/include)
target_link_libraries(${ECOSYSTEM_EXE} PRIVATE ${ECOSYSTEM_LIB} dl)
