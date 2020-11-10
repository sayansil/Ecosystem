#ifndef STAT_FETCHER_HPP
#define STAT_FETCHER_HPP

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <filesystem>
#include <iostream>
#include <animal.hpp>
#include <plant.hpp>
#include <map_maker.hpp>
#include <schema.hpp>
#include <folly/container/F14Map.h>

using ENTITY_MAP_TYPE = folly::F14FastMap<std::string, ENTITY>;

namespace stat_fetcher
{
    extern std::unordered_map<std::string, unsigned int> get_kind_distribution(const ENTITY_MAP_TYPE &);
    extern std::vector<PStat> get_one_stat(const ENTITY_MAP_TYPE &, const std::string &, const std::string & = "");
    extern std::pair<double, double> get_stat_gap(const ENTITY_MAP_TYPE &, const std::string &, const std::string & = "");
    extern std::pair<unsigned int, unsigned int> get_matable_population(const ENTITY_MAP_TYPE &, const std::string & = "");
    extern std::string prepare_data_for_simulation(const ENTITY_MAP_TYPE &);
    extern std::vector<std::map<std::string, std::string>> prepare_data_for_simulation_2(const ENTITY_MAP_TYPE &);
    extern unsigned int get_population(const ENTITY_MAP_TYPE &, const std::string & = "");
    extern double get_stat_average(const ENTITY_MAP_TYPE &, const std::string &, const std::string & = "");
    extern double get_gender_ratio(const ENTITY_MAP_TYPE &, const std::string & = "");
    extern std::vector<DBType> get_db_row(const ENTITY_MAP_TYPE &, const std::string &, const std::string &, const unsigned int &, std::unordered_map<std::string, std::unordered_map<StatGroup, std::vector<std::string>>> &);
}; // namespace stat_fetcher

#endif // STAT_FETCHER_HPP
