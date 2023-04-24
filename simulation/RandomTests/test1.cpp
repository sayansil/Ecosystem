#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <population_generated.h>

#include <database_manager.hpp>
#include <ecosystem_types.hpp>
#include <god.hpp>
#include <inspector.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <range/v3/view.hpp>
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <vector>

int main() {
    namespace rv = ranges::views;
    std::vector<std::vector<ByteArray>> rows;
    const size_t simulation_years = 1;

    setup::setup(helper::get_ecosystem_root());

    const size_t initial_organism_count = 8;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (auto i : rv::iota(0u, initial_organism_count / 2)) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
        organisms.push_back(
            {{"kind", "bamboo"}, {"kingdom", "1"}, {"age", "20"}});
    }

    {
        God allah(helper::get_ecosystem_root(), true);
        allah.cleanSlate();
        allah.createWorld(organisms);
        for (auto i : rv::iota(0u, simulation_years)) {
            allah.happy_new_year(true);
        }

        nlohmann::ordered_json query;
        // sample extraction query
        //{
        //    query["type"] = "extract";
        //    nlohmann::json species1;
        //    species1["kind"] = "deer";
        //    species1["kingdom"] = "Animal";
        //    nlohmann::json organism1;
        //    organism1.emplace_back("base_weight");
        //    organism1.emplace_back("base_height");
        //    organism1.emplace_back("age");
        //    species1["organism"] = std::move(organism1);
        //    query["species"].emplace_back(std::move(species1));

        //    nlohmann::json species2;
        //    species2["kind"] = "bamboo";
        //    species2["kingdom"] = "Plant";
        //    nlohmann::json organism2;
        //    organism2.emplace_back("age");
        //    organism2.emplace_back("height");
        //    species2["organism"] = std::move(organism2);
        //    query["species"].emplace_back(std::move(species2));
        //}

        // sample filter query
        {
            query["type"] = "filter";
            nlohmann::json species1;
            species1["kind"] = "deer";
            species1["kingdom"] = "Animal";
            nlohmann::json organism1;
            organism1["base_weight"]["low"] = 1;
            organism1["base_weight"]["high"] = 5;
            organism1["base_height"]["low"] = 0;
            organism1["base_height"]["high"] = 100;
            species1["organism"].emplace_back(std::move(organism1));
            query["species"].emplace_back(std::move(species1));

            nlohmann::json species2;
            species2["kind"] = "bamboo";
            species2["kingdom"] = "Plant";
            nlohmann::json organism2;
            organism2["age"]["low"] = 1;
            organism2["age"]["high"] = 25;
            species2["organism"].emplace_back(std::move(organism2));
            query["species"].emplace_back(std::move(species2));
        }

        fmt::print("query: {}\n", query.dump(4));
        EcosystemInspector inspector(allah.buffer.data(), query);
        inspector.Inspect();
        fmt::print("{}\n", inspector.output.dump(4));
    }
}
