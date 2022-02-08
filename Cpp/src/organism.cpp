#include <organism.hpp>
#include <helper.hpp>

static void increment_age_animal(Ecosystem::Organism *organism);
static void increment_age_plant(Ecosystem::Organism *organism);
static void evaluate_static_fitness_animal(Ecosystem::Organism *organism);
static void evaluate_static_fitness_plant(Ecosystem::Organism *organism);

namespace organism_opts
{
    void increment_age(Ecosystem::Organism *organism)
    {
        if (organism->kingdom() == Ecosystem::KingdomE::Animal)
        {
            increment_age_animal(organism);
        }
        else if (organism->kingdom() == Ecosystem::KingdomE::Plant)
        {
            increment_age_plant(organism);
        }
        else
        {
            throw std::runtime_error("Unknown kingdom");
        }
    }

    void evaluate_static_fitness(Ecosystem::Organism *organism)
    {
        if (organism->kingdom() == Ecosystem::KingdomE::Animal)
        {
            evaluate_static_fitness_animal(organism);
        }
        else if (organism->kingdom() == Ecosystem::KingdomE::Plant)
        {
            evaluate_static_fitness_plant(organism);
        }
        else
        {
            throw std::runtime_error("Unknown kingdom");
        }
    }
};

void increment_age_animal(Ecosystem::Organism *organism)
{
}

void increment_age_plant(Ecosystem::Organism *organism)
{
}

void evaluate_static_fitness_animal(Ecosystem::Organism *organism)
{
}

void evaluate_static_fitness_plant(Ecosystem::Organism *organism)
{
}
