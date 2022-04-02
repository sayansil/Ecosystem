#define CATCH_CONFIG_MAIN
#include <god.hpp>
#include <iostream>
#include <memory>
#include <catch2/catch.hpp>

TEST_CASE("Test with no database", "[test_nodb]")
{
    bool flag = true;
    REQUIRE( flag == true );
}

TEST_CASE("Test with master database", "[test_masterdb]")
{
    bool flag = false;

    REQUIRE( flag == true );
}

TEST_CASE("Test with stats database", "[test_statsdb]")
{
    bool flag = true;

    REQUIRE( flag == true );
}
