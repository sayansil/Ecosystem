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

    std::pair<double, double> getStatGap(const ANIMAL_MAP_TYPE &animals, const std::string &attribute)
    {
        double low = 0.0, high = 0.0, value;
        STAT current_attribute;
        bool uninitialized = true;
        for (const auto &animal : animals)
        {
            current_attribute = animal.second.get_stat(attribute);
            if(current_attribute.data.index() == 0)
            {
                value = (unsigned int)current_attribute;
            }
            else if(current_attribute.data.index() == 1)
            {
                value = current_attribute;
            }
            else
            {
                break;
            }
            if (uninitialized)
            {
                low = value;
                high = value;
                uninitialized = false;
            }
            else
            {
                if (low > value)
                {
                    low = value;
                }
                if (high < value)
                {
                    high = value;
                }
            }
        }

        return {low, high};
    }

    std::unordered_map<std::string, unsigned int> getKindDistribution(const ANIMAL_MAP_TYPE &animals)
    {
        std::unordered_map<std::string, unsigned int> kindDistribution;

        for (const auto &animal: animals)
        {
            if(kindDistribution.find(animal.second.kind) == kindDistribution.end())
            {
                kindDistribution[animal.second.kind] = 1;
            }
            else
            {
                kindDistribution[animal.second.kind]++;
            }
        }

        return kindDistribution;
    }

    double getStatAverage(const ANIMAL_MAP_TYPE &animals, const std::string &attribute)
    {
        double average = 0.0, n = 0.0, value = 0.0;

        // avg(n+1) = (n / (n + 1)) * avg(n) + (1 / (n + 1)) * kn

        for (const auto &animal: animals)
        {
            value = animal.second.get_stat(attribute);
            average = (n / (n + 1)) * average + (value / (n + 1));
            n++;
        }

        return average;
    }

    std::vector<STAT> getOneStat(const ANIMAL_MAP_TYPE &animals, const std::string &attribute)
    {
        std::vector<STAT> attributeList;

        for (const auto &animal : animals)
        {
            attributeList.push_back(animal.second.get_stat(attribute));
        }

        return attributeList;
    }
};
