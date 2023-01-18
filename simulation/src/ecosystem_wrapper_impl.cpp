#include <ecosystem_wrapper_impl.hpp>
#include <god.hpp>
#include <setup.hpp>
#include <memory>

uint32_t initial_organism_count;
uint32_t simulation_years;
std::vector<std::unordered_map<std::string, std::string>> organisms;
God * god;
std::filesystem::path root_path;

void create_god_impl(uint8_t gods_eye) {
    root_path = setup::setup();
    god = new God(root_path, 0);
}

void set_initial_parms_impl(uint32_t count, uint32_t years) {
    initial_organism_count = count;
    simulation_years = years;
    organisms.reserve(initial_organism_count);
    for (size_t i = 0; i < initial_organism_count; i++)
    {
        organisms.push_back({{"kind", "deer"},
                            {"kingdom", "0"},
                            {"age", "20"}});
    }
}

void create_world_impl() {
    god->createWorld(organisms);
}

void run_simulation_impl() {
    god->cleanSlate();
    god->createWorld(organisms);
    for (size_t i = 0; i < simulation_years; i++) {
        god->happy_new_year(true);
    }

}

void free_god_impl() {
    delete god;
}
