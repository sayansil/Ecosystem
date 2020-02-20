#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <vector>

#include <animal.hpp>

#define ANIMAL_MAP_TYPE std::unordered_map<std::string, Animal>

namespace stat_fetcher
{
    extern double getMFratio(const ANIMAL_MAP_TYPE &);
};

#endif // STAT_FETCHER_HPP
