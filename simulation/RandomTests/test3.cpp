#include <fmt/core.h>

#include <nlohmann/json.hpp>
#include <vector>

int main() {
    nlohmann::ordered_json query;
    query.push_back("lol1");
    query.push_back("lol2");
    query.push_back("lol3");

    for (const auto& item : query.items()) {
        fmt::print("{}\n", item.value());
    }
}
