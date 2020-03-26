#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <string>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include <organism.hpp>
#include <filesystem>

using ORGANISM_MAP_TYPE = std::unordered_map<std::string, ORGANISM>;

namespace stat_fetcher
{
    extern std::unordered_map<std::string, unsigned int> getKindDistribution(const ORGANISM_MAP_TYPE &);
    extern std::pair<double, double> getStatGap(const ORGANISM_MAP_TYPE &, const std::string &, const std::string & = "");
    extern double getStatAverage(const ORGANISM_MAP_TYPE &, const std::string &, const std::string & = "");
    extern std::vector<STAT> getOneStat(const ORGANISM_MAP_TYPE &, const std::string &, const std::string & = "");
    extern double getGenderRatio(const ORGANISM_MAP_TYPE &, const std::string & = "");
    extern std::pair<unsigned int, unsigned int> getMatablePopulation(const ORGANISM_MAP_TYPE &, const std::string & = "");
    extern unsigned int getPopulation(const ORGANISM_MAP_TYPE &, const std::string & = "");
    extern std::unordered_map<std::string, double> getAllStats(const ORGANISM_MAP_TYPE &, const std::string & = "");
    extern std::string generateDataForPy(const ORGANISM_MAP_TYPE&);
}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
