FetchContent_Declare(
    catch
    GIT_REPOSITORY "https://github.com/DarkStar1997/Catch"
    GIT_TAG 10885319b67e85299691128198000eed935026bd
    GIT_PROGRESS TRUE
)
FetchContent_Populate(catch)

add_executable(tests tests.cpp)
target_include_directories(tests PRIVATE ${Ecosystem_SOURCE_DIR}/include 
                                            ${catch_SOURCE_DIR})
target_link_libraries(tests PRIVATE ${ECOSYSTEM_LIB} dl)
add_test(NAME EcosystemTest COMMAND tests)
