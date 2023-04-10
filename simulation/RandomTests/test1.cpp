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

        nlohmann::ordered_json query = {
            {"type", "extract"},
            {"species",
             {{{"kind", "deer"},
               {"kingdom", "Animal"},
               {"organism",
                {"base_weight",
                 //"base_weight", {{"low", 1}, {"high", 5}},
                 "age", "height", "weight"}}},
              {{"kind", "bamboo"},
               {"kingdom", "Plant"},
               {"organism", {"age"}}}}}};

        EcosystemInspector inspector(allah.buffer.data(), query);
        inspector.IterateWorld();
        fmt::print("{}\n", inspector.output.dump(4));
    }
}
