#include <fmt/core.h>

#include <god.hpp>
#include <setup.hpp>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::vector<FBuffer>> rows;
    const size_t simulation_years = 250;

    std::filesystem::path ecosystem_root = helper::get_ecosystem_root();
    setup::setup(ecosystem_root);

    const size_t initial_organism_count = 500;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
    }

    God allah(ecosystem_root, true);
    allah.cleanSlate();
    allah.createWorld(organisms);
    for (size_t i = 0; i < simulation_years; i++) {
        allah.happy_new_year();
    }

    fmt::print("Simulation ended.\n");
}
