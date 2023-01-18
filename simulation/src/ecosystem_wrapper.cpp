#include <ecosystem_wrapper.hpp>
#include <god.hpp>
#include <memory>
#include <setup.hpp>
#include <vector>
#include <filesystem>

uint32_t initial_organism_count;
uint32_t simulation_years;
std::vector<std::unordered_map<std::string, std::string>> organisms;
std::unique_ptr<God> god;
std::filesystem::path root_path;

void create_god(uint8_t gods_eye) {
    root_path = setup::setup();
    god = std::make_unique<God>(root_path, 0);
}

void set_initial_parms(uint32_t count, uint32_t years) {
    initial_organism_count = count;
    simulation_years = years;
    organisms.reserve(initial_organism_count);
    for (size_t i = 0; i < initial_organism_count; i++) {
        organisms.push_back(
            {{"kind", "deer"}, {"kingdom", "0"}, {"age", "20"}});
    }
}

void create_world() { god->createWorld(organisms); }

void run_simulation() {
    god->cleanSlate();
    god->createWorld(organisms);
    for (size_t i = 0; i < simulation_years; i++) {
        god->happy_new_year(true);
    }
}
