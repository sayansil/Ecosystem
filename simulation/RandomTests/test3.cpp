#include <fmt/core.h>

#include <nlohmann/json.hpp>
#include <vector>

int main() {
    nlohmann::ordered_json query;

    nlohmann::json species_query1;
    species_query1["kind"] = "deer";
    species_query1["kingdom"] = "Animal";
    query["species"].emplace_back(species_query1);

    nlohmann::json species_query2;
    species_query2["kind"] = "bamboo";
    species_query2["kingdom"] = "Plant";
    query["species"].emplace_back(species_query2);

    fmt::print("{}\n", query.dump(4));
}
