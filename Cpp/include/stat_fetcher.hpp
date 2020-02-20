#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <vector>
#include <utility>

#include <animal.hpp>

#define ANIMAL_MAP_TYPE std::unordered_map<std::string, Animal>

namespace stat_fetcher
{
    extern double getMFratio(const ANIMAL_MAP_TYPE &);
    extern std::pair<unsigned int, unsigned int> getMatablePopulation(const ANIMAL_MAP_TYPE &);
    extern unsigned int getPopulation(const ANIMAL_MAP_TYPE &);

}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
