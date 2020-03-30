#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <organism.hpp>
#include <filesystem>
#include <iostream>
#include <animal.hpp>
#include <plant.hpp>

using ENTITY_MAP_TYPE = std::unordered_map<std::string, ENTITY>;

namespace stat_fetcher
{
    extern std::unordered_map<std::string, unsigned int> getKindDistribution(const ENTITY_MAP_TYPE &);
    extern std::unordered_map<std::string, double> getAllStats(const ENTITY_MAP_TYPE &, const std::string & = "");
    extern std::vector<STAT> getOneStat(const ENTITY_MAP_TYPE &, const std::string &, const std::string & = "");
    extern std::pair<double, double> getStatGap(const ENTITY_MAP_TYPE &, const std::string &, const std::string & = "");
    extern std::pair<unsigned int, unsigned int> getMatablePopulation(const ENTITY_MAP_TYPE &, const std::string & = "");
    extern std::string prepareDataForSimulation(const ENTITY_MAP_TYPE &);
    extern unsigned int getPopulation(const ENTITY_MAP_TYPE &, const std::string & = "");
    extern double getStatAverage(const ENTITY_MAP_TYPE &, const std::string &, const std::string & = "");
    extern double getGenderRatio(const ENTITY_MAP_TYPE &, const std::string & = "");
}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
