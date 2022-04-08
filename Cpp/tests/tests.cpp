#define CATCH_CONFIG_MAIN
#include <god.hpp>
#include <iostream>
#include <memory>
#include <setup.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <flatbuffers/idl.h>
#include <fmt/core.h>
#include <flatbuffers/minireflect.h>
#include <unordered_map>
#include <stat_fetcher.hpp>
#include <database_manager.hpp>
#include <ecosystem_types.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Setup", "[test_setup]")
{
    REQUIRE_NOTHROW(setup::setup());
}

TEST_CASE("Create world without db", "[test_cworld_nodb]")
{
    REQUIRE_NOTHROW([&]()
                    {
        setup::setup();

        unsigned int initial_organism_count = 500;
        std::vector<std::unordered_map<std::string, std::string>> organisms;
        organisms.reserve(initial_organism_count);

        for (size_t i = 0; i < initial_organism_count; i++)
        {
            organisms.push_back({{"kind", "deer"},
                                 {"kingdom", "0"},
                                 {"age", "20"}});
        }

        God allah;
        allah.cleanSlate();
        allah.createWorld(organisms);
        fmt::print("Buffer size = {:.2f}MB\n", allah.buffer.size() / (1024.0 * 1024)); }());
}

TEST_CASE("Create world with db", "[test_cworld_db]")
{

    std::vector<FBuffer> rows;

    REQUIRE_NOTHROW([&]()
                    {
        setup::setup();
        
        unsigned int initial_organism_count = 500;
        std::vector<std::unordered_map<std::string, std::string>> organisms;
        organisms.reserve(initial_organism_count);

        for (size_t i = 0; i < initial_organism_count; i++)
        {
            organisms.push_back({{"kind", "deer"},
                                    {"kingdom", "0"},
                                    {"age", "20"}});
        }

        God allah;
        allah.cleanSlate();
        allah.createWorld(organisms);
        fmt::print("Buffer size = {:.2f}MB\n", allah.buffer.size() / (1024.0 * 1024));

        FBuffer buff = stat_fetcher::create_avg_world(allah.buffer);

        DatabaseManager db_manager;
        db_manager.insert_rows({buff});
        rows = db_manager.read_all_rows();
        fmt::print("Row size: {}\n", rows.size()); }());

    REQUIRE(rows.size() == 1);
}
