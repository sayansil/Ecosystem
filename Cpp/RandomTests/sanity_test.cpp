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
    setup::setup();

    //unsigned int initial_organism_count = 4500;
    unsigned int initial_organism_count = 4000;
    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++)
    {
        organisms.push_back({{"kind", "deer"},
                             {"kingdom", "0"},
                             {"age", "20"}});
    }

    FBuffer avg_instance;

    // Keep God (contains db object) and other db operations in separate scopes
    {
        God allah;
        allah.cleanSlate();
        allah.createWorld(organisms);
        fmt::print("Buffer size = {:.2f}MB\n", allah.buffer.size() / (1024.0 * 1024));

        flatbuffers::ToStringVisitor visitor("", true, "", true);
        flatbuffers::IterateFlatBuffer(allah.buffer.data(), Ecosystem::WorldTypeTable(), &visitor);
        fmt::print("Iterated result: {}\n", visitor.s);
        nlohmann::json json_data = nlohmann::json::parse(visitor.s);
        fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));

        // avg_instance = stat_fetcher::create_avg_world(allah.buffer);
    }

    //{
    //    DatabaseManager db_manager;

    //    db_manager.insert_rows({avg_instance});

    //    std::vector<FBuffer> rows = db_manager.read_all_rows();
    //    fmt::print("Rows: {}\n", rows.size());

    //    flatbuffers::ToStringVisitor visitor("", true, "", true);
    //    flatbuffers::IterateFlatBuffer(rows[0].data(), Ecosystem::WorldTypeTable(), &visitor);
    //    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    //    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
    //}
}
