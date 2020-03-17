#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <string>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include <animal.hpp>
#include <filesystem>

using ANIMAL_MAP_TYPE = std::unordered_map<std::string, Animal>;

namespace stat_fetcher
{
    extern std::unordered_map<std::string, unsigned int> getKindDistribution(const ANIMAL_MAP_TYPE &);
    extern std::pair<double, double> getStatGap(const ANIMAL_MAP_TYPE &, const std::string &, const std::string & = "");
    extern double getStatAverage(const ANIMAL_MAP_TYPE &, const std::string &, const std::string & = "");
    extern std::vector<STAT> getOneStat(const ANIMAL_MAP_TYPE &, const std::string &, const std::string & = "");
    extern double getGenderRatio(const ANIMAL_MAP_TYPE &, const std::string & = "");
    extern std::pair<unsigned int, unsigned int> getMatablePopulation(const ANIMAL_MAP_TYPE &, const std::string & = "");
    extern unsigned int getPopulation(const ANIMAL_MAP_TYPE &, const std::string & = "");
    extern std::unordered_map<std::string, double> getAllStats(const ANIMAL_MAP_TYPE &, const std::string & = "");
    extern std::string generateDataForPy(const ANIMAL_MAP_TYPE&);
}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
