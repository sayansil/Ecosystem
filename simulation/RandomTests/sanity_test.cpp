#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <database_manager.hpp>
#include <ecosystem_types.hpp>
#include <god.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>
#include <vector>

int main() {
    setup::setup();

    const size_t initial_organism_count = 500;
    const size_t simulation_years = 10;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
    }

    God allah;
    allah.cleanSlate();
    allah.createWorld(organisms);
    for (size_t i = 1; i <= simulation_years; i++) {
        allah.happy_new_year(false);
    }
    fmt::print("Buffer size = {:.2f}MB\n",
               allah.buffer.size() / (1024.0 * 1024));
}
