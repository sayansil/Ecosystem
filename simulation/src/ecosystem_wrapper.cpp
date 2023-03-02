#include <ecosystem_wrapper.hpp>
#include <filesystem>
#include <god.hpp>
#include <memory>
#include <setup.hpp>
#include <vector>

std::vector<std::unordered_map<std::string, std::string>> organisms;
std::unique_ptr<God> god;

void create_god(uint8_t gods_eye, const char *ecosystem_root) {
    std::filesystem::path root_path = setup::setup(ecosystem_root);
    god = std::make_unique<God>(root_path, gods_eye);
}

void set_initial_organisms(uint32_t kingdom, const char *kind, uint32_t age,
                           uint32_t count) {
    organisms.reserve(organisms.capacity() + count);

    for (size_t i = 0; i < count; i++) {
        organisms.push_back({{"kind", std::string(kind)},
                             {"kingdom", std::to_string(kingdom)},
                             {"age", std::to_string(age)}});
    }
}

void create_world() { god->createWorld(organisms); }

void run_simulation(uint32_t years) {
    god->cleanSlate();
    god->createWorld(organisms);
    for (size_t i = 0; i < years; i++) {
        god->happy_new_year(true);
    }
}
