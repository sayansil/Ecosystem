#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <population_generated.h>

#include <avg_plot_handler.hpp>
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
    const size_t simulation_years = 10;

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
        AveragePlotHandler plots;
        for (auto i : rv::iota(0u, simulation_years)) {
            allah.happy_new_year(true);
            plots.add_record(allah.avg_buffer, allah.population_stats);
            fmt::print("{}\n", plots.plot_history.dump(4));
        }
        /*
        const Ecosystem::World* const world =
            Ecosystem::GetWorld(allah.buffer.data());
        const auto& species = world->species()->Get(0);
        const auto& organism = species->organism()->Get(0);
        fmt::print("{}\n", organism->name()->str());
        const uint8_t* organism_ptr = species->organism()->GetAs<uint8_t>(0);
        flatbuffers::ToStringVisitor visitor("", true, "", false);
        flatbuffers::IterateObject(organism_ptr, Ecosystem::OrganismTypeTable(),
                                   &visitor);
        nlohmann::json organism_json = nlohmann::json::parse(visitor.s);
        fmt::print("{}\n", organism_json.dump(4));
        */
    }
}
