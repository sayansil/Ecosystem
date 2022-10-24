#include <god.hpp>
#include <setup.hpp>
#include <vector>
#include <fmt/core.h>
#include <unordered_map>

int main()
{
    std::vector<std::vector<FBuffer>> rows;
    const size_t simulation_years = 250;

    setup::setup();

    const size_t initial_organism_count = 500;

    std::vector<std::unordered_map<std::string, std::string>> organisms;
    organisms.reserve(initial_organism_count);

    for (size_t i = 0; i < initial_organism_count; i++)
    {
        organisms.push_back({{"kind", "deer"},
                            {"kingdom", "0"},
                            {"age", "20"}});
    }

    God allah(true);
    allah.cleanSlate();
    allah.createWorld(organisms);
    for (size_t i = 0; i < simulation_years; i++) {
        allah.happy_new_year();
    }

    fmt::print("Simulation ended.\n");
}
