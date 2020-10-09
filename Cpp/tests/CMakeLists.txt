download_file(
  https://github.com/catchorg/Catch2/releases/download/v2.0.1/catch.hpp
  ${Ecosystem_SOURCE_DIR}/tests/catch/catch.hpp
  SHA256 6988c82def026a0b862f43ab122bbbd6051b0b7648bfe04c8a150ef62ca5a0a2)


add_executable(tests tests.cpp)
target_include_directories(tests PRIVATE ${Ecosystem_SOURCE_DIR}/include catch/)
target_link_libraries(tests PRIVATE ${ECOSYSTEM_LIB} dl)
add_test(NAME EcosystemTest COMMAND tests)
