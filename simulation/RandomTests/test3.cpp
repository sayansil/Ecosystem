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

    FBuffer buff = stat_fetcher::create_avg_world(allah.buffer);

    {
        DatabaseManager db_manager;

        db_manager.insert_rows({buff});

        std::vector<FBuffer> rows = db_manager.read_all_rows();

        flatbuffers::ToStringVisitor visitor("", true, "", true);
        flatbuffers::IterateFlatBuffer(rows[0].data(), Ecosystem::WorldTypeTable(), &visitor);
        nlohmann::json json_data = nlohmann::json::parse(visitor.s);
        fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
    }
}
