#include <god.hpp>
#include <setup.hpp>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <flatbuffers/idl.h>
#include <fmt/core.h>
#include <flatbuffers/minireflect.h>
#include <unordered_map>
#include <stat_fetcher.hpp>
#include <database_manager.hpp>
#include <ecosystem_types.hpp>

int main()
{
    setup::setup();

    unsigned int initial_organism_count = 4500;
    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++)
    {
        organisms.push_back({{"kind", "deer"},
                             {"kingdom", "0"},
                             {"age", "20"}});
    }

    God allah;
    // Cannot create 2 db instances in same context
    // allah.cleanSlate();
    allah.createWorld(organisms);
    fmt::print("Buffer size = {:.2f}MB\n", allah.buffer.size() / (1024.0 * 1024));
    allah.happy_new_year();
}
