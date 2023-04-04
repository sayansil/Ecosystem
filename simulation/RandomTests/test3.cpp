#include <fmt/core.h>

#include <nlohmann/json.hpp>
#include <vector>

// void processQuery(const nlohmann::json& organism_queries) {
//     for (const auto& organism_query : organism_queries.items()) {
//         if (organism_query.value().is_structured()) {
//             //fmt::print("{} -- {}\n", organism_query.key(),
//             organism_query.value().dump());
//             //auto organism_stat = organism_query.value().begin();
//             //fmt::print("{}\n", organism_stat.key());
//             //fmt::print("{}\n",
//             organism_query.value()[organism_stat.key()]);
//             //for (const auto& i : organism_query.value().items()) {
//             //    fmt::print("{} - {}\n", i.key(), i.value().dump());
//             //}
//             nlohmann::json current_query = organism_query.value();
//             fmt::print("{}\n", current_query.dump());
//             fmt::print("{}\n", current_query.items().begin().key());
//             fmt::print("{}\n",
//             current_query.items().begin().value()["high"].get<float>());
//             fmt::print("{}\n",
//             current_query.items().begin().value()["low"].get<float>());
//         }
//     }
// }

void processQuery(const nlohmann::json& organism_queries) {
    for (const auto& index : organism_queries.items()) {
        for (const auto& value : index.value().items()) {
            if (value.value().is_structured()) {
                std::string organism_stat = value.key();
                fmt::print("stat - {}\n", organism_stat);
                for (const auto& stat_bounds : value.value().items()) {
                    fmt::print("{} - {}\n", stat_bounds.key(),
                               stat_bounds.value().get<float>());
                }
            }
        }
    }
}

int main() {
    nlohmann::ordered_json query;
    query["type"] = "filter";

    nlohmann::json species_query1;
    species_query1["kind"] = "deer";
    species_query1["kingdom"] = "Animal";
    species_query1["organism"].push_back(
        nlohmann::json::parse(R"({"age":{"low":1,"high":5}})"));
    species_query1["organism"].push_back("height");
    species_query1["organism"].push_back("weight");
    processQuery(species_query1["organism"]);
    query["species"].emplace_back(species_query1);

    nlohmann::json species_query2;
    species_query2["kind"] = "bamboo";
    species_query2["kingdom"] = "Plant";
    species_query2["organism"].push_back("age");
    query["species"].emplace_back(species_query2);

    // fmt::print("{}\n", query.dump(4));
}
