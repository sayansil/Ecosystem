#include <stat_fetcher.hpp>

std::vector<Animal> getAnimalVector(const ANIMAL_MAP_TYPE &animals)
{
    std::vector<Animal> animals_vec;
    for (auto &animal : animals)
    {
        animals_vec.push_back(animal.second);
    }

    return animals_vec;
}

namespace stat_fetcher
{
    double getMFratio(const ANIMAL_MAP_TYPE &animals)
    {
        unsigned int M = 0, F = 0;
        for (auto animal : animals)
        {
            if (animal.second.gender == MALE)
            {
                M++;
            }
            else
            {
                F++;
            }
        }

        return (1.0 * M) / F;
    }

    unsigned int getPopulation(const ANIMAL_MAP_TYPE &animals)
    {
        return animals.size();
    }

    std::pair<unsigned int, unsigned int> getMatablePopulation(const ANIMAL_MAP_TYPE &animals)
    {
        unsigned int M = 0, F = 0;

        for (const auto &animal: animals)
        {
            if (animal.second.age >= animal.second.mating_age_start && animal.second.age <= animal.second.mating_age_end)
            {
                if (animal.second.gender == MALE)
                {
                    M++;
                }
                else
                {
                    F++;
                }
            }
        }

        return {M, F};
    }
};