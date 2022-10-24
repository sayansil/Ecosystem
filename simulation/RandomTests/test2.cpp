#include <god.hpp>
#include <setup.hpp>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <flatbuffers/idl.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <flatbuffers/minireflect.h>
#include <unordered_map>
#include <stat_fetcher.hpp>
#include <database_manager.hpp>
#include <ecosystem_types.hpp>
#include <population_generated.h>

int main()
{
    std::vector<std::vector<FBuffer>> rows;
    const size_t simulation_years = 100;

    setup::setup();

    const size_t initial_organism_count = 500;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++)
    {
        organisms.push_back({{"kind", "deer"},
                            {"kingdom", "0"},
                            {"age", "20"}});
    }

    {
        God allah(true);
        allah.cleanSlate();
        allah.createWorld(organisms);
        for (size_t i = 0; i < simulation_years; i++) {
            allah.happy_new_year(true);
        }
    }

    {
        DatabaseManager db_manager;
        rows = db_manager.read_all_rows(); 
    }

    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(rows[0][1].data(), Ecosystem::WorldPopulationTypeTable(), &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}
