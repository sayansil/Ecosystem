#define CATCH_CONFIG_MAIN
#include <god.hpp>
#include <iostream>
#include <memory>
#include <catch.hpp>

TEST_CASE("Test with no database", "[test_nodb]")
{
    bool flag;

    try
    {
        unsigned int initial_organism_count = 200;
        unsigned int years_to_simulate = 100;

        God *allah = new God();
        allah->reset_species("plant/bamboo");

        while (initial_organism_count--)
        {
            allah->spawn_organism(std::make_shared<Plant>("bamboo", 10, false, "OG-" + std::to_string(initial_organism_count)));
        }

        while (years_to_simulate--)
        {
            allah->happy_new_year();
        }

        delete allah;

        flag = true;
    }
    catch(const std::exception& e)
    {
        std::cout << "test_nodb failed." << '\n';
        std::cout << e.what() << '\n';
        flag = false;
    }

    REQUIRE( flag == true );
}

TEST_CASE("Test with master database", "[test_masterdb]")
{
    bool flag;

    try
    {
        unsigned int initial_organism_count = 200;
        unsigned int years_to_simulate = 100;

        God *allah = new God(true);
        allah->reset_species("plant/bamboo");

        while (initial_organism_count--)
        {
            allah->spawn_organism(std::make_shared<Plant>("bamboo", 10, false, "OG-" + std::to_string(initial_organism_count)));
        }

        while (years_to_simulate--)
        {
            allah->happy_new_year();
        }

        delete allah;
        DatabaseManager* db = new DatabaseManager();
        auto rows = db->read_all_rows_stats("plant/bamboo");

        flag = true;

        delete db;
    }
    catch (const std::exception &e)
    {
        std::cout << "test_masterdb failed." << '\n';
        std::cout << e.what() << '\n';
        flag = false;
    }

    REQUIRE( flag == true );
}

TEST_CASE("Test with stats database", "[test_statsdb]")
{
    bool flag;

    try
    {
        unsigned int initial_organism_count = 200;
        unsigned int years_to_simulate = 100;

        God *allah = new God();
        allah->reset_species("plant/bamboo");

        while (initial_organism_count--)
        {
            allah->spawn_organism(std::make_shared<Plant>("bamboo", 10, false, "OG-" + std::to_string(initial_organism_count)));
        }

        unsigned int yts = years_to_simulate;
        while (yts--)
        {
            allah->happy_new_year();
            allah->remember_species("plant/bamboo");
        }

        delete allah;

        DatabaseManager* db = new DatabaseManager();
        auto rows = db->read_all_rows_stats("plant/bamboo");

        if (rows.size() == years_to_simulate)
        {
            flag = true;
        }
        else
        {
            std::cout << "test_statsdb failed." << '\n';
            std::cout << "Did not write to STATS table." << '\n';
            flag = false;
        }
        delete db;
    }
    catch (const std::exception &e)
    {
        std::cout << "test_statsdb failed." << '\n';
        std::cout << e.what() << '\n';
        flag = false;
    }

    REQUIRE( flag == true );
}
