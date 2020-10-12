add_executable(tests tests.cpp)
target_include_directories(tests PRIVATE ${Ecosystem_SOURCE_DIR}/include ${Ecosystem_SOURCE_DIR}/extern/nlohmann/include ${Ecosystem_SOURCE_DIR}/extern/catch)
target_link_libraries(tests PRIVATE ${ECOSYSTEM_LIB} dl)
add_test(NAME EcosystemTest COMMAND tests)
