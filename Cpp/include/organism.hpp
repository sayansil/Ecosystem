#ifndef ORGANISM_HPP
#define ORGANISM_HPP

#include <world_generated.h>

namespace organism_opts
{
    extern void increment_age(Ecosystem::Organism *organism);
    extern void evaluate_static_fitness(Ecosystem::Organism *organism);
    extern void generate_death_factor(Ecosystem::Organism *organism);
    extern bool is_normal_child(Ecosystem::Organism *organism);
};

#endif /* ORGANISM_HPP */
