#ifndef SPECIES_CONSTANTS_HPP
#define SPECIES_CONSTANTS_HPP

#include <nlohmann/json.hpp>

namespace constants
{
    extern nlohmann::json species_constants_map;
    extern void init();
};

#endif /* SPECIES_CONSTANTS_HPP */
