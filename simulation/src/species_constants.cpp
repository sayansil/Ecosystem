#include <fmt/core.h>

#include <filesystem>
#include <helper.hpp>
#include <species_constants.hpp>

static nlohmann::json species_constants_map;

namespace constants {

nlohmann::json& get_species_constants_map() { return species_constants_map; }

static void parse_species_directories(const std::filesystem::path dirpath) {
    if (std::filesystem::exists(dirpath)) {
        for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
            // entry.path() / "base.json" copy to map
            std::string species_name = entry.path().filename().string();

            if (species_name[0] == '.')
                // non-species file (probably hidden file/directory)
                continue;

            std::ifstream in(entry.path() / "base.json");
            nlohmann::json tmp;
            in >> tmp;
            in.close();
            species_constants_map[species_name] = tmp;
        }
    }
}

void init(const std::filesystem::path& ecosystem_root) {
    parse_species_directories(ecosystem_root / "data" / "json" / "animal");
    parse_species_directories(ecosystem_root / "data" / "json" / "plant");
}
};  // namespace constants
