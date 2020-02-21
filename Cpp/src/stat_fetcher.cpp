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

    std::pair<stat_type, stat_type> getStatGap(const ANIMAL_MAP_TYPE &animals, const std::string &attribute)
    {
        stat_type low=0.0, high=0.0, current_attribute;
        bool uninitialized = true;
        for (const auto &animal : animals)
        {
            current_attribute = animal.second.get_stat(attribute);
            if (uninitialized)
            {
                low = current_attribute;
                high = current_attribute;
                uninitialized = false;
            }
            else
            {
                if (low > current_attribute)
                {
                    low = current_attribute;
                }
                if (high < current_attribute)
                {
                    high = current_attribute;
                }
            }
        }

        return {low, high};
    }
};