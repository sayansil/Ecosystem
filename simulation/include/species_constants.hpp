#ifndef SPECIES_CONSTANTS_HPP
#define SPECIES_CONSTANTS_HPP

#include <nlohmann/json.hpp>
#include <helper.hpp>

namespace constants
{
    extern nlohmann::json& get_species_constants_map();
    extern void init(const std::filesystem::path & ecosystem_root = helper::get_ecosystem_root());
};

#endif /* SPECIES_CONSTANTS_HPP */
