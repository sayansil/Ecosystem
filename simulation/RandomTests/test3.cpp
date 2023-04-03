#include <fmt/core.h>

#include <nlohmann/json.hpp>
#include <vector>

int main() {
    nlohmann::json j;
    nlohmann::json species1;
    species1["stat1"] = 4;
    nlohmann::json species2;
    species2["stat1"] = 5;
    j["species"].emplace_back(species1);
    j["species"].emplace_back(species2);
    fmt::print("{}\n", j.dump(4));
}
