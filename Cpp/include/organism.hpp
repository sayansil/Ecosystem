#ifndef ORGANISM_HPP
#define ORGANISM_HPP

#include <world_generated.h>

namespace organism_opts
{
    extern double get_value_from_chromosome(const Ecosystem::Organism *organism, const std::string &code, const double &multiplier);

    extern double get_immunity_from_chromosome(const Ecosystem::Organism *organism);
    extern double get_base_vitality(const Ecosystem::Organism *organism);
    extern double get_base_weight(const Ecosystem::Organism *organism);
    extern double get_height_multiplier(const Ecosystem::Organism *organism);
    extern double get_vitality_multiplier(const Ecosystem::Organism *organism);
    extern double get_weight_multiplier(const Ecosystem::Organism *organism);
    extern double get_max_height(const Ecosystem::Organism *organism);
    extern double get_max_weight(const Ecosystem::Organism *organism);
    extern int8_t get_gender(const Ecosystem::Organism *organism);
    extern double get_max_weight(const Ecosystem::Organism *organism);
    extern double get_base_height(const Ecosystem::Organism *organism);
    extern double get_base_appetite(const Ecosystem::Organism *organism);
    extern double get_base_speed(const Ecosystem::Organism *organism);
    extern double get_base_stamina(const Ecosystem::Organism *organism);
    extern double get_speed_multiplier(const Ecosystem::Organism *organism);
    extern double get_stamina_multiplier(const Ecosystem::Organism *organism);
};

#endif /* ORGANISM_HPP */
