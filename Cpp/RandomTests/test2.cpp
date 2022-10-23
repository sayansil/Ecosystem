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

int main()
{
    std::vector<FBuffer> rows;
    const size_t simulation_years = 100;

    setup::setup();

    const size_t initial_organism_count = 5000;

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

    fmt::print("Years: {}, Rows: {}\n", simulation_years, rows.size());
}
