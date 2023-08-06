#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <population_generated.h>

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
    std::vector<std::vector<ByteArray>> rows;
    const size_t simulation_years = 100;

    setup::setup(helper::get_ecosystem_root());

    const size_t initial_organism_count = 500;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
    }

    {
        God allah(helper::get_ecosystem_root(), true);
        allah.cleanSlate();
        allah.createWorld(organisms);
        for (size_t i = 0; i < simulation_years; i++) {
            allah.happy_new_year(true);
        }

        flatbuffers::ToStringVisitor visitor("", true, "", true);
        flatbuffers::IterateFlatBuffer(allah.avg_buffer.data(),
                                       Ecosystem::WorldTypeTable(), &visitor);
        nlohmann::json json_data = nlohmann::json::parse(visitor.s);
        fmt::print("Parsed JSON:\n{}\n",
                   json_data["species"][0]["organism"][0].dump(4));

        fmt::print("stat-name\tis_number\tis_object\tis_array\n");

        for (const auto& [key, value] :
             json_data["species"][0]["organism"][0].items()) {
            fmt::print("{}\t{}\t{}\t{}\n", key, value.is_number(),
                       value.is_object(), value.is_array());
        }
    }

    {
        DatabaseManager db_manager(
            std::filesystem::path(helper::get_ecosystem_root()) /
            "data/ecosystem_master.db");
        rows = db_manager.read_all_rows();
    }

    flatbuffers::ToStringVisitor visitor("", true, "", true);
    flatbuffers::IterateFlatBuffer(
        rows[0][1].data(), Ecosystem::WorldPopulationTypeTable(), &visitor);
    nlohmann::json json_data = nlohmann::json::parse(visitor.s);
    fmt::print("Parsed JSON:\n{}\n", json_data.dump(4));
}
