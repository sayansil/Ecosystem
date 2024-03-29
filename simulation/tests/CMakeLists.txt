FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.8
)
FetchContent_MakeAvailable(Catch2)

function(add_ecosystem_tests TARGET_NAME TEST_NAME)
  add_executable(${TARGET_NAME} ${TARGET_NAME}.cpp)
  target_include_directories(${TARGET_NAME} PRIVATE ${Ecosystem_SOURCE_DIR}/include ${catch_SOURCE_DIR})

  if(WIN32 OR APPLE OR ANDROID)
    target_link_libraries(${TARGET_NAME} PRIVATE ${ECOSYSTEM_LIB} fmt::fmt Catch2::Catch2)
  else()
    target_link_libraries(${TARGET_NAME} PRIVATE ${ECOSYSTEM_LIB} fmt::fmt dl pthread Catch2::Catch2)
  endif()

  copy_dll(${TARGET_NAME} fmt)
  copy_dll(${TARGET_NAME} ${ECOSYSTEM_LIB})

  add_test(NAME ${TEST_NAME} COMMAND ${TARGET_NAME})
endfunction()

add_ecosystem_tests(tests EcosystemTest)
add_ecosystem_tests(stress_tests EcosystemStressTest)
