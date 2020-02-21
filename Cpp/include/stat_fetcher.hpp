#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <vector>
#include <utility>

#include <animal.hpp>

using ANIMAL_MAP_TYPE = std::unordered_map<std::string, Animal>;

namespace stat_fetcher
{
    extern std::unordered_map<std::string, unsigned int> getKindDistribution(const ANIMAL_MAP_TYPE &);
    extern std::pair<STAT_TYPE, STAT_TYPE> getStatGap(const ANIMAL_MAP_TYPE &, const std::string &);
    extern double getStatAverage(const ANIMAL_MAP_TYPE &, const std::string &);
    extern std::vector<STAT_TYPE> getOneStat(const ANIMAL_MAP_TYPE &, const std::string &);
    extern double getMFratio(const ANIMAL_MAP_TYPE &);
    extern std::pair<unsigned int, unsigned int> getMatablePopulation(const ANIMAL_MAP_TYPE &);
    extern unsigned int getPopulation(const ANIMAL_MAP_TYPE &);
}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
