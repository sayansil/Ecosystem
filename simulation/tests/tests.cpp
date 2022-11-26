#define CATCH_CONFIG_MAIN
#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>

#include <catch2/catch.hpp>
#include <database_manager.hpp>
#include <ecosystem_types.hpp>
#include <god.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <vector>

TEST_CASE("Setup", "[test_setup]") { REQUIRE_NOTHROW(setup::setup()); }

TEST_CASE("Create world without db", "[test_cworld_nodb]") {
    REQUIRE_NOTHROW([&]() {
        setup::setup();

        const size_t initial_organism_count = 100;

        std::vector<std::unordered_map<std::string, std::string>> organisms;
        organisms.reserve(initial_organism_count);

        for (size_t i = 0; i < initial_organism_count; i++) {
            organisms.push_back(
                {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
        }

        God allah;
        allah.cleanSlate();
        allah.createWorld(organisms);
        allah.happy_new_year(false);
    }());
}

TEST_CASE("Create world with db", "[test_cworld_db]") {
    std::vector<std::vector<ByteArray>> rows;
    const size_t simulation_years = 20;

    REQUIRE_NOTHROW([&]() {
        setup::setup();

        const size_t initial_organism_count = 100;

        std::vector<std::unordered_map<std::string, std::string>> organisms;
        organisms.reserve(initial_organism_count);

        for (size_t i = 0; i < initial_organism_count; i++) {
            organisms.push_back(
                {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
        }

        {
            God allah(true);
            allah.cleanSlate();
            allah.createWorld(organisms);
            for (size_t i = 0; i < simulation_years; i++) {
                allah.happy_new_year(false);
            }
        }

        {
            DatabaseManager db_manager;
            rows = db_manager.read_all_rows();
        }
    }());

    REQUIRE(rows.size() == simulation_years);
}
