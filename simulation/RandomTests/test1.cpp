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
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::vector<ByteArray>> rows;
    const size_t simulation_years = 1;

    setup::setup(helper::get_ecosystem_root());

    const size_t initial_organism_count = 8;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count / 2; i++) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
    }

    for (size_t i = 0; i < initial_organism_count / 2; i++) {
        organisms.push_back(
            {{"kind", "bamboo"}, {"kingdom", "1"}, {"age", "20"}});
    }

    {
        God allah(helper::get_ecosystem_root(), true);
        allah.cleanSlate();
        allah.createWorld(organisms);
        for (size_t i = 0; i < simulation_years; i++) {
            allah.happy_new_year(true);
        }

        nlohmann::ordered_json query;

        nlohmann::json species_query1;
        species_query1["kind"] = "deer";
        species_query1["kingdom"] = "Animal";
        query["species"].emplace_back(species_query1);

        // nlohmann::json species_query2;
        // species_query2["kind"] = "bamboo";
        // species_query2["kingdom"] = "Plant";
        // query["species"].emplace_back(species_query2);

        EcosystemInspector inspector(allah.buffer.data(), query);
        inspector.IterateWorld();
        fmt::print("{}\n", inspector.output.dump(4));
    }
}
