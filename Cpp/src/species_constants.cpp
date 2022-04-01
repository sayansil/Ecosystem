#include <species_constants.hpp>
#include <helper.hpp>
#include <filesystem>

namespace constants
{
    nlohmann::json species_constants_map;

    static void parse_species_directories(const std::filesystem::path dirpath)
    {
        if (std::filesystem::exists(dirpath))
        {
            for (const auto &entry : std::filesystem::directory_iterator(dirpath))
            {
                // entry.path() / "base.json" copy to map

                std::string species_name = entry.path().filename().string();
                std::ifstream in(entry.path() / "base.json");
                nlohmann::json tmp;
                in >> tmp;
                in.close();
                species_constants_map[species_name] = tmp;
            }
        }
    }

    void init()
    {
        parse_species_directories(helper::get_ecosystem_root() / "data" / "json" / "animal");
        parse_species_directories(helper::get_ecosystem_root() / "data" / "json" / "plant");
    }
};
