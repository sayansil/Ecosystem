#ifndef SPECIES_CONSTANTS_HPP
#define SPECIES_CONSTANTS_HPP

#include <ecosystem_types.hpp>
#include <helper.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>

namespace constants
{
    extern nlohmann::json species_constants_map;
    extern void init();
};

#endif /* SPECIES_CONSTANTS_HPP */
